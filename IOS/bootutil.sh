#!/usr/bin/env bash
# xdrabbo00 Boris Nicolas Dráb
# 2025-04-06

export POSIXLY_CORRECT=yes  # pokud chcete psat pro posix shell misto bashe, zmente shebang na #!/bin/sh a odkomentujte tento radek
export LC_ALL=C              

BOOT_ENTRIES_DIR="/boot/loader/entries"
COMMAND=""
SHOW_PATH_ONLY="false"

while getopts "b:f" opt; do 
    case "$opt" in
    b)
        BOOT_ENTRIES_DIR="$OPTARG"
        ;;
    f)
        SHOW_PATH_ONLY="true"
        ;;
    *)
        echo "Neplatný argument!" >&2
        exit 42
        ;;
    esac
done
shift $((OPTIND - 1))

if [[ -z "$1" ]]; then
    echo "Chyba: Žiadny príkaz nebol zadaný!"
    exit 42
else
    COMMAND="$1"
    shift
fi

if [[ ! -d "$BOOT_ENTRIES_DIR" ]]; then
    echo "Chyba: Adresár '$BOOT_ENTRIES_DIR' neexistuje!" >&2
    exit 42
fi

ARGS=("$@")

# Function: List files in a directory
list_files() {
    local dir="$1"

    if [[ ! -d "$dir" ]]; then
        echo "Chyba: Adresár '$dir' neexistuje alebo je neplatný!" >&2
        exit 42
    fi

    find "$dir" -type f -name "*.conf" -print0 | xargs -0 -I{} echo {}
}

# Function: Filter files by title regex
filter_by_title() {
    local regex="$1"

    while IFS= read -r file; do
        awk -v regex="$regex" '
            /^title / { title=substr($0, index($0, $2)) }
            END {
                if (title ~ regex) {
                    print FILENAME
                }
            }
        ' "$file"
    done
}

# Function: Filter files by kernel regex
filter_by_kernel() {
    local regex="$1"

    while IFS= read -r file; do
        awk -v regex="$regex" '
            /^linux / { linux=substr($0, index($0, $2)) }
            END {
                if (linux ~ regex) {
                    print FILENAME
                }
            }
        ' "$file"
    done
}

# Function: Format the output
format_list() {
    while IFS= read -r file; do
        awk '
            /^title / { title=substr($0, index($0, $2)) }
            /^version / { version=substr($0, index($0, $2)) }
            /^linux / { linux=substr($0, index($0, $2)) }
            END {
                print title " (" version ", " linux ")"
            }
        ' "$file"
    done
}

# Function: List entries with filters and sorting
list_entries() {
    local dir="$1"
    shift
    local title_regex=".*"
    local kernel_regex=".*"
    local sort_type=""
    local entries_with_sortkey=()
    local entries_without_sortkey=()

    # Parse parameters
    while [[ "$#" -gt 0 ]]; do
        case "$1" in
            -t)
                title_regex="$2"
                shift 2
                ;;
            -k)
                kernel_regex="$2"
                shift 2
                ;;
            -s)
                sort_type="sortkey"
                shift
                ;;
            -f)
                sort_type="filename"
                shift
                ;;
            *)
                echo "Neplatný parameter '$1'!" >&2
                exit 42
                ;;
        esac
    done

    # Validate directory
    if [[ ! -d "$dir" ]]; then
        echo "Chyba: Adresár '$dir' neexistuje alebo je neplatný!" >&2
        exit 42
    fi

    # Process files
    while IFS= read -r -d '' file; do
        local sort_key=""
        local title=""
        local version=""
        local linux=""
        local filename=$(basename "$file")

        # Extract fields using awk
        sort_key=$(awk '/^sort-key / { print $2 }' "$file")
        title=$(awk '/^title / { print substr($0, index($0, $2)) }' "$file")
        version=$(awk '/^version / { print $2 }' "$file")
        linux=$(awk '/^linux / { print $2 }' "$file")

        # Apply regex filters
        if [[ -n "$kernel_regex" && ! "$linux" =~ $kernel_regex ]]; then
            continue
        fi
        if [[ -n "$title_regex" && ! "$title" =~ $title_regex ]]; then
            continue
        fi

        # Build the entry
        local entry="$sort_key|$filename|$title ($version, $linux)"
        if [[ -n "$sort_key" ]]; then
            entries_with_sortkey+=("$entry")
        else
            entries_without_sortkey+=("|$filename|$title ($version, $linux)")
        fi
    done < <(find "$dir" -type f -name "*.conf" -print0)

    # Sorting logic
    if [[ "$sort_type" == "sortkey" ]]; then
        sorted_with_sortkey=$(printf "%s\n" "${entries_with_sortkey[@]}" | sort -t'|' -k1,1 -k2,2)
        sorted_without_sortkey=$(printf "%s\n" "${entries_without_sortkey[@]}" | sort -t'|' -k2,2)
        output=$(printf "%s\n%s\n" "$sorted_with_sortkey" "$sorted_without_sortkey")
    elif [[ "$sort_type" == "filename" ]]; then
        output=$(printf "%s\n" "${entries_with_sortkey[@]}" "${entries_without_sortkey[@]}" | sort -t'|' -k2,2)
    else
        output=$(printf "%s\n" "${entries_with_sortkey[@]}" "${entries_without_sortkey[@]}")
    fi

    # Output results without sort-key
    echo "$output" | cut -d'|' -f3-
}


# Function: Show the default boot entry
show_default() {
    local boot_entries_dir="$1"
    local show_path_only=${SHOW_PATH_ONLY:-false}
    local default_file=""

    if [[ -z "$boot_entries_dir" || ! -d "$boot_entries_dir" ]]; then
        echo "Chyba: Adresár '$boot_entries_dir' neexistuje alebo je neplatný!" >&2
        exit 42
    fi

    while IFS= read -r file; do
        if grep -q "^vutfit_default y$" "$file"; then
            default_file="$file"
            break
        fi
    done < <(find "$boot_entries_dir" -type f -name "*.conf")

    if [[ -z "$default_file" ]]; then
        echo "Chyba: Predvolený súbor nebol nájdený!" >&2
        exit 42
    fi

    if [[ "$show_path_only" == "true" ]]; then
        realpath "$default_file"
    else
        cat "$default_file" || {
            echo "Chyba: Nepodarilo sa prečítať obsah súboru!" >&2
            exit 42
        }
    fi
}

# Function: Set a file as the default boot entry
make_default() {
    local boot_entries_dir="$1"
    local entered_file_path="$2"

    if [[ ! -f "$entered_file_path" ]]; then
        echo "Chyba: Súbor '$entered_file_path' neexistuje!" >&2
        exit 42
    fi

    for default_file in "$boot_entries_dir"/*conf; do
        if grep -q "^vutfit_default y$" "$default_file"; then
            sed -i 's/^vutfit_default y$/vutfit_default n/' "$default_file"
        fi
    done

    if ! grep -q "^vutfit_default" "$entered_file_path"; then
        echo "vutfit_default y" >> "$entered_file_path"
    else
        sed -i 's/^vutfit_default .*/vutfit_default y/' "$entered_file_path"
    fi

    echo "Súbor '$entered_file_path' bol nastavený ako predvolený zavádzací súbor."
}

# Function: Remove boot entries matching a title regex
remove() {
    local title_regex="$1"

    if [[ -z "$title_regex" ]]; then
        echo "Chyba: Nebol zadaný regulárny výraz." >&2
        exit 42
    fi

    for file in "$BOOT_ENTRIES_DIR"/*.conf; do
        if [[ -f "$file" ]]; then
            title_line=$(grep "^title " "$file" | cut -d' ' -f2-)
            if [[ $title_line =~ $title_regex ]]; then
                rm "$file"
            fi
        fi
    done
}

duplicate() {
    local entry_file_path=""
    local kernel_path=""
    local initramfs_path=""
    local new_title=""
    local add_cmdline_args=()
    local remove_cmdline_args=()
    local destination=""
    local make_default=false

    # Parse parameters
    while [[ "$#" -gt 0 ]]; do
        case "$1" in
            -k)
                kernel_path="$2"
                shift 2
                ;;
            -i)
                initramfs_path="$2"
                shift 2
                ;;
            -t)
                new_title="$2"
                shift 2
                ;;
            -a)
                add_cmdline_args+=("$2")
                shift 2
                ;;
            -r)
                remove_cmdline_args+=("$2")
                shift 2
                ;;
            -d)
                destination="$2"
                shift 2
                ;;
            --make-default)
                make_default=true
                shift
                ;;
            *)
                if [[ -z "$entry_file_path" ]]; then
                    entry_file_path="$1"
                    shift
                else
                    echo "Neplatný parameter: $1" >&2
                    exit 42
                fi
                ;;
        esac
    done

    # If no entry file path is specified, use the default entry
    if [[ -z "$entry_file_path" ]]; then
        entry_file_path=$(find "$BOOT_ENTRIES_DIR" -type f -name "*.conf" -exec grep -l "^vutfit_default y$" {} +)
        if [[ -z "$entry_file_path" ]]; then
            echo "Chyba: Žiadny výchozí záznam neexistuje a nebola zadaná cesta k súboru!" >&2
            exit 42
        fi
    fi

    # Verify the source file exists
    if [[ ! -f "$entry_file_path" ]]; then
        echo "Chyba: Súbor '$entry_file_path' neexistuje!" >&2
        exit 42
    fi

    # Determine destination file name if not specified
    if [[ -z "$destination" ]]; then
        destination="${entry_file_path%.*}-copy.conf"
        local counter=1
        while [[ -f "$destination" ]]; do
            destination="${entry_file_path%.*}-copy-$counter.conf"
            counter=$((counter + 1))
        done
    fi

    # Copy the original file to the destination
    cp "$entry_file_path" "$destination"

    # Modify the copied file based on parameters
    [[ -n "$kernel_path" ]] && sed -i "s|^linux .*|linux $kernel_path|" "$destination"
    [[ -n "$initramfs_path" ]] && sed -i "s|^initrd .*|initrd $initramfs_path|" "$destination"
    [[ -n "$new_title" ]] && sed -i "s|^title .*|title $new_title|" "$destination"

    # Add or remove command-line arguments
    for arg in "${add_cmdline_args[@]}"; do
        sed -i "/^options / s/$/ $arg/" "$destination"
    done
    for arg in "${remove_cmdline_args[@]}"; do
        sed -i "/^options / s/ $arg//g" "$destination"
    done

    # Handle --make-default
    if $make_default; then
        for file in "$BOOT_ENTRIES_DIR"/*.conf; do
            sed -i 's/^vutfit_default y$/vutfit_default n/' "$file"
        done
        sed -i 's/^vutfit_default .*/vutfit_default y/' "$destination"
        if ! grep -q "^vutfit_default" "$destination"; then
            echo "vutfit_default y" >> "$destination"
        fi
    fi

    echo "Nový záznam bol vytvorený: $destination"
}

    


# Main command dispatcher
case "$COMMAND" in 
    list)
        list_entries "$BOOT_ENTRIES_DIR" "${ARGS[@]}"
        ;;
    remove)
        remove "${ARGS[0]}"
        ;;
    duplicate)
        duplicate_entry "${ARGS[0]}" "${ARGS[@]:1}"
        ;;
    show-default)
        show_default "$BOOT_ENTRIES_DIR"
        ;;
    make-default)
        make_default "$BOOT_ENTRIES_DIR" "${ARGS[0]}"
        ;;
    *)
        echo "Chyba: Neznámy príkaz '$COMMAND'!" >&2
        exit 42
        ;;
esac


########################################################################################################

#!/usr/bin/env bash
# xdrabbo00 Boris Nicolas Dráb
# 2025-04-06

export POSIXLY_CORRECT=yes  # pokud chcete psat pro posix shell misto bashe, zmente shebang na #!/bin/sh a odkomentujte tento radek
export LC_ALL=C              

BOOT_ENTRIES_DIRECTORY="/boot/loader/entries"
COMMAND=""
SHOW_PATH_ONLY="false"

while getopts "b:f" opt; do 
    case "$opt" in
    b)
        BOOT_ENTRIES_DIRECTORY="$OPTARG"
        ;;
    f)
        SHOW_PATH_ONLY="true"
        ;;
    *)
        echo "Invalid argument " >&2
        exit 42
        ;;
    esac
done

shift $((OPTIND - 1))

if [[ -z "$1" ]]; then
    echo "Error - No command entered "
    exit 42
else
    COMMAND="$1"
    shift
fi

if [[ ! -d "$BOOT_ENTRIES_DIRECTORY" ]]; then
    echo "Error - Director does not exist " >&2
    exit 42
fi


ARGUMENTS=("$@")


list_files() {
    local directory="$1"

    if [[ ! -d "$directory" ]]; then
        echo "Error - Directory does not exist or is invalid " >&2
        exit 42
    fi

    find "$directory" -type f -name "*.conf" -print0 | xargs -0 -I{} echo {}
}


filter_by_title() {
    local regex="$1"

    while IFS= read -r file; do
        awk -v regex="$regex" '
            /^title / { title=substr($0, index($0, $2)) }
            END {
                if (title ~ regex) {
                    print FILENAME
                }
            }
        ' "$file"
    done
}


filter_by_kernel() {
    local regex="$1"

    while IFS= read -r file; do
        awk -v regex="$regex" '
            /^linux / { linux=substr($0, index($0, $2)) }
            END {
                if (linux ~ regex) {
                    print FILENAME
                }
            }
        ' "$file"
    done
}


format_list() {
    while IFS= read -r file; do
        awk '
            /^title / { title=substr($0, index($0, $2)) }
            /^version / { version=substr($0, index($0, $2)) }
            /^linux / { linux=substr($0, index($0, $2)) }
            END {
                print title " (" version ", " linux ")"
            }
        ' "$file"
    done
}


list_entries() {
    local dir="$1"

    shift

    local sort_type=""
    local title_regex=".*"
    local kernel_regex=".*"

    local entries_with_sortkey=()
    local entries_without_sortkey=()

    #Parse parameters
    while [[ "$#" -gt 0 ]]; do
        case "$1" in
            -t)
                title_regex="$2"
                shift 2
                ;;
            -k)
                kernel_regex="$2"
                shift 2
                ;;
            -s)
                sort_type="sortkey"
                shift
                ;;
            -f)
                sort_type="filename"
                shift
                ;;
            *)
                echo "Invalid parameter " >&2
                exit 42
                ;;
        esac
    done

    #Validate directory
    if [[ ! -d "$dir" ]]; then
        echo "Error - Directory does not exist or is invalid " >&2
        exit 42
    fi

    #Process files
    while IFS= read -r -d '' file; do
        local sort_key=""
        local title=""
        local version=""
        local linux=""
        local filename=$(basename "$file")

        #Extract fields using awk
        sort_key=$(awk '/^sort-key / { print $2 }' "$file")
        title=$(awk '/^title / { print substr($0, index($0, $2)) }' "$file")
        version=$(awk '/^version / { print $2 }' "$file")
        linux=$(awk '/^linux / { print $2 }' "$file")

        # Apply regex filters
        if [[ -n "$kernel_regex" && ! "$linux" =~ $kernel_regex ]]; then
            continue
        fi
        if [[ -n "$title_regex" && ! "$title" =~ $title_regex ]]; then
            continue
        fi

        #Build the entry
        local entry="$sort_key|$filename|$title ($version, $linux)"
        if [[ -n "$sort_key" ]]; then
            entries_with_sortkey+=("$entry")
        else
            entries_without_sortkey+=("|$filename|$title ($version, $linux)")
        fi
    done < <(find "$dir" -type f -name "*.conf" -print0)

    #Sorting logic
    if [[ "$sort_type" == "sortkey" ]]; then
        sorted_with_sortkey=$(printf "%s\n" "${entries_with_sortkey[@]}" | sort -t'|' -k1,1 -k2,2)
        sorted_without_sortkey=$(printf "%s\n" "${entries_without_sortkey[@]}" | sort -t'|' -k2,2)
        output=$(printf "%s\n%s\n" "$sorted_with_sortkey" "$sorted_without_sortkey")
    elif [[ "$sort_type" == "filename" ]]; then
        output=$(printf "%s\n" "${entries_with_sortkey[@]}" "${entries_without_sortkey[@]}" | sort -t'|' -k2,2)
    else
        output=$(printf "%s\n" "${entries_with_sortkey[@]}" "${entries_without_sortkey[@]}")
    fi

    #Results without sort-key
    echo "$output" | cut -d'|' -f3-
}


show_default() {
    local default_file=""
    local boot_entries_dir="$1"
    local show_path_only=${SHOW_PATH_ONLY:-false}

    if [[ -z "$boot_entries_dir" || ! -d "$boot_entries_dir" ]]; then
        echo "Error - Repository doesn't exist or is invalid " >&2
        exit 42
    fi

    while IFS= read -r file; do
        if grep -q "^vutfit_default y$" "$file"; then
            default_file="$file"
            break
        fi
    done < <(find "$boot_entries_dir" -type f -name "*.conf")

    if [[ -z "$default_file" ]]; then
        echo "Error - Choosen file wasn't found " >&2
        exit 42
    fi

    if [[ "$show_path_only" == "true" ]]; then
        realpath "$default_file"
    else
        cat "$default_file" || {
            echo "Error - Failed to read file " >&2
            exit 42
        }
    fi
}


make_default() {
    local boot_entries_dir="$1"
    local entered_file_path="$2"

    if [[ ! -f "$entered_file_path" ]]; then
        echo "Error - File doesn't exist " >&2
        exit 42
    fi

    for default_file in "$boot_entries_dir"/*conf; do
        if grep -q "^vutfit_default y$" "$default_file"; then
            sed -i 's/^vutfit_default y$/vutfit_default n/' "$default_file"
        fi
    done

    if ! grep -q "^vutfit_default" "$entered_file_path"; then
        echo "vutfit_default y" >> "$entered_file_path"
    else
        sed -i 's/^vutfit_default .*/vutfit_default y/' "$entered_file_path"
    fi

    echo "File has been set as the default boot file "
}


remove() {
    local title_regex="$1"

    if [[ -z "$title_regex" ]]; then
        echo "Error - No regular expression was specified " >&2
        exit 42
    fi

    for file in "$BOOT_ENTRIES_DIRECTORY"/*.conf; do
        if [[ -f "$file" ]]; then
            title_line=$(grep "^title " "$file" | cut -d' ' -f2-)
            if [[ $title_line =~ $title_regex ]]; then
                rm "$file"
            fi
        fi
    done
}


duplicate() {
    local entry_file_path=""
    local kernel_path=""
    local initramfs_path=""
    local new_title=""
    local add_cmdline_args=()
    local remove_cmdline_args=()
    local destination=""
    local make_default=false

    #Parse arguments
    while [[ "$#" -gt 0 ]]; do
        case "$1" in
            -k)
                kernel_path="$2"
                shift 2
                ;;
            -i)
                initramfs_path="$2"
                shift 2
                ;;
            -t)
                new_title="$2"
                shift 2
                ;;
            -a)
                add_cmdline_args+=("$2")
                shift 2
                ;;
            -r)
                remove_cmdline_args+=("$2")
                shift 2
                ;;
            -d)
                destination="$2"
                shift 2
                ;;
            --make-default)
                make_default=true
                shift
                ;;
            *)
                if [[ -z "$entry_file_path" ]]; then
                    entry_file_path="$1"
                    shift
                else
                    echo "Invalid parameter: $1" >&2
                    exit 42
                fi
                ;;
        esac
    done

    #If no entry file path is specified, locate the default entry
    if [[ -z "$entry_file_path" ]]; then
        entry_file_path=$(find "$BOOT_ENTRIES_DIRECTORY" -type f -name "*.conf" -exec grep -l "^vutfit_default y$" {} +)
        if [[ -z "$entry_file_path" ]]; then
            echo "Chyba: Žiadny výchozí záznam neexistuje a nebola zadaná cesta k súboru!" >&2
            exit 42
        fi
    fi

    #Validate the existence of the source file
    if [[ ! -f "$entry_file_path" ]]; then
        echo "Chyba: Súbor '$entry_file_path' neexistuje!" >&2
        exit 42
    fi

    #Generate a unique name for the destination file if not specified
    if [[ -z "$destination" ]]; then
        destination="${entry_file_path%.*}.1.conf"
        local counter=2
        while [[ -f "$destination" ]]; do
            destination="${entry_file_path%.*}.$counter.conf"
            counter=$((counter + 1))
        done
    fi

    #Copy the original file to the destination
    cp "$entry_file_path" "$destination"

    #Modify the copied file based on provided arguments
    [[ -n "$new_title" ]] && sed -i "s|^title .*|title $new_title|" "$destination"
    [[ -n "$kernel_path" ]] && sed -i "s|^linux .*|linux $kernel_path|" "$destination"
    [[ -n "$initramfs_path" ]] && sed -i "s|^initrd .*|initrd $initramfs_path|" "$destination"

    #Handle kernel command-line arguments: add and remove
    for arg in "${add_cmdline_args[@]}"; do
        sed -i "/^options / s/$/ $arg/" "$destination"
    done
    for arg in "${remove_cmdline_args[@]}"; do
        sed -i "/^options / s/ $arg//g" "$destination"
    done

    #Set the newly created entry as default
    if $make_default; then
        for file in "$BOOT_ENTRIES_DIRECTORY"/*.conf; do
            sed -i 's/^vutfit_default y$/vutfit_default n/' "$file"
        done
        sed -i 's/^vutfit_default .*/vutfit_default y/' "$destination"
        if ! grep -q "^vutfit_default" "$destination"; then
            echo "vutfit_default y" >> "$destination"
        fi
    fi

    #Output the created entry file path
    echo "Nový záznam bol vytvorený: $destination"
}

    
#Processing commands
case "$COMMAND" in 
    list)
        list_entries "$BOOT_ENTRIES_DIRECTORY" "${ARGUMENTS[@]}"
        ;;
    #duplicate)
        #duplicate_entry "${ARGUMENTS[0]}" "${ARGUMENTS[@]:1}"
        #;;
    remove)
        remove "${ARGUMENTS[0]}"
        ;;
    make-default)
        make_default "$BOOT_ENTRIES_DIRECTORY" "${ARGUMENTS[0]}"
        ;;
    show-default)
        show_default "$BOOT_ENTRIES_DIRECTORY"
        ;;
    *)
        echo "Error - Unknown command " >&2
        exit 42
        ;;
esac













fi

if [[ ! -f "default_entry" ]]; then
    echo "ERROR3"
     exit 1

fi

filename=$(basename "$default_entry" .conf)
destilation="${BOOT_ENTRIES_DIRECTORY}/${filename}.conf"

while [[ -f  "$destination" ]]; do
    destilation="${BOOT_ENTRIES_DIRECTORY}/${filename}.${i}_copy.conf"
     ((i++))
done

cp -- "$default_entry" "$destination" || {echo "Error: failed to copy."; exit 1;  }

if $process_add_first; then

if [[  "${add_params[@]}" -gt 0 ]]; then
   for param in "${add_params[@]}"; do

         if ! grep -q "^options.*\b$param\b" "$destination"; then
                 sad -i "/^options/s/$/ $param/" "$destination"
         fi
   done
fi