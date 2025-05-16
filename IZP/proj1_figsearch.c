// xdrabbo00

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Define constants for various errors and statuses.
#define VALID 0
#define INVALID_FILE 1
#define INVALID_ARGS 2
#define NUM_OF_ARGS 3
#define MEMORY_ERROR 4

// Function prototypes.
void process (const char *filename, const char *operation, int *result);

/**
 * Main function to start the program.
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return Status code indicating the result of the program execution.
 */
int main(int argc, char *argv[]) {
    // Check if the number of arguments is correct.
    if (argc != NUM_OF_ARGS) {
        fprintf (stderr, "Invalid arguments\n");
        return INVALID_ARGS;
    }

    int result;
    // Process the input file and operation.
    process(argv[2], argv[1], &result);

    // Handle the result.
    switch (result) {
        case VALID:
            if (strcmp (argv[1], "test") == 0) {
                fprintf (stdout, "Valid\n");
            }
            break;

        case INVALID_FILE:
            fprintf (stderr, "Invalid\n");
            break;

        case MEMORY_ERROR:
            fprintf (stderr, "Allocation failure\n");
            break;

        default:
            fprintf (stderr, "Unkown error\n");
            break;
    }

    return result;
}

/**  
 * Function to chcek the validity of the bitmap file and read its content.
 * @param filename Name of the text file.
 * @param num_rows Pointer to store the number of rows.
 * @param num_cols Pointer to store the number of columns.
 * @param bitmap Pointer to store the 2D array of the bitmap.
 * @return VALID if the file is valid, 
 *         INVALID_FILE if the file is invalid,
 *         MEMORY_ERROR if there is a memory allocation failure.
*/
int validity_check(const char *filename, int *num_rows, int *num_cols, int ***bitmap) {

    int value = 0;
    int row_count = 0;

    // Open the file for reading.
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return INVALID_FILE;
    }

    // Read the number of rows and columns.
    if (fscanf (file, "%d %d", num_rows, num_cols) != 2 || *num_rows <= 0 || *num_cols <= 0) {
        fclose (file);
        return INVALID_FILE;
    }

    // Allocate memory for the bitmap. 
    *bitmap = malloc (*num_rows * sizeof(int *));
    if (*bitmap == NULL) {
        fclose (file);
        return MEMORY_ERROR;
    }

    // Allocate memory for each row of the bitmap.
    for (int index  = 0; index < *num_rows; index++) {
        (*bitmap)[index] = malloc(*num_cols * sizeof(int));
        if ((*bitmap)[index] == NULL) {
            for (int index2 = 0; index2 < index; index2++) {
                free ((*bitmap)[index2]);
            }
            free (*bitmap);
            fclose (file);
            return MEMORY_ERROR;
        }
    }

    // Read the bitmap values.
    while (row_count < *num_rows) {
        for (int index = 0; index < *num_cols; index++) {
            if (fscanf(file, "%d", &value) != 1) {
                fclose (file);
                return INVALID_FILE;
            }
            if (value != 0 && value != 1) {
                fclose (file);
                return INVALID_FILE;
            }
            (*bitmap)[row_count][index] = value;

        }
        row_count++;
    }

    // Check for extra characters after reading bitmap values.
    int c;
    do {
        c = getc(file);
    } while (isblank(c));
    
    if (c != EOF && c != '\n' && c != '\r') {
        fclose (file);
        return INVALID_FILE;
    }

    fclose (file);
    return VALID;
}

/**  
 * Function to find the longest horizontal line in the bitmap.
 * @param num_rows Number of rows in the bitmap.
 * @param num_cols Number of columns in the bitmap.
 * @param bitmap 2D array of the bitmap.
 * @param start_x Pointer to store the starting x-coordinates of the line.
 * @param start_y Pointer to store the starting y-coordinates of the line.
 * @param end_x Pointer to store the ending x-coordinates of the line.
 * @param end_y Pointer to store the ending y-coordinates of the line.
 * @return Lenght of the longest horizontal line found. 
 * */
int find_hline (int num_rows, int num_cols, int **bitmap, int *start_x, int *start_y, int *end_x, int *end_y) {
    int max_lenght = 0;
    int current_lenght = 0;
    int current_start_x, current_start_y;

    for (int index = 0; index < num_rows; index++) {
        current_lenght = 0;
        for (int index2 = 0; index2 < num_cols; index2++) {
            if (bitmap[index][index2] == 1) {
                if (current_lenght == 0) {
                    current_start_x = index;
                    current_start_y = index2;
                }
                current_lenght++;

                if (current_lenght > max_lenght || (current_lenght == max_lenght && (current_start_x < *start_x || (current_start_x == *start_x && current_start_y < *start_y)))){
                    max_lenght = current_lenght;
                    *start_x = current_start_x;
                    *start_y = current_start_y;
                    *end_x = index;
                    *end_y = index2;
                }

            } else {
                current_lenght = 0;
            }
        }
    }
    return max_lenght;
}

/**
 *  Function to find the longest vertical line in the bitmap.
 * @param num_rows Number of rows in the bitmap.
 * @param num_cols Number of columns in the bitmap.
 * @param bitmap 2D array of the bitmap.
 * @param start_x Pointer to store the starting x-coordinates of the line.
 * @param start_y Pointer to store the starting y-coordinates of the line.
 * @param end_x Pointer to store the ending x-coordinates of the line.
 * @param end_y Pointer to store the ending y-coordinates of the line.
 * @return Lenght of the longest vertical line found. 
 */
int find_vline (int num_rows, int num_cols, int **bitmap, int *start_x, int *start_y, int *end_x, int *end_y) {
    int max_lenght = 0;
    int current_lenght = 0;
    int current_start_x, current_start_y;

    for (int index2 = 0; index2 < num_cols; index2++) {
        current_lenght = 0;
        for (int index = 0; index < num_rows; index++) {
            if (bitmap[index][index2] == 1) {
                if (current_lenght == 0) {
                    current_start_x = index;
                    current_start_y = index2;
                }
                current_lenght++; 

                if (current_lenght > max_lenght || (current_lenght == max_lenght && (current_start_x < *start_x || (current_start_x == *start_x &&  current_start_y < *start_y)))) {
                    max_lenght = current_lenght;
                    *start_x = current_start_x;
                    *start_y = current_start_y;
                    *end_x = index;
                    *end_y = index2;
                }

            } else {
                current_lenght = 0;
            }
        }
    }
    return max_lenght;
}

/**
 *  Function to find the largest square in the bitmap.
 * @param num_rows Number of rows in the bitmap.
 * @param num_cols Number of columns in the bitmap.
 * @param bitmap 2D array of the bitmap.
 * @param start_x Pointer to store the starting x-coordinates of the square.
 * @param start_y Pointer to store the starting y-coordinates of the square.
 * @param end_x Pointer to store the ending x-coordinates of the square.
 * @param end_y Pointer to store the ending y-coordinates of the square.
 * @return Size of the largest square found. 
 */
int find_square (int num_rows, int num_cols, int **bitmap, int *start_x, int *start_y, int *end_x, int *end_y) {
    int maximum_size = 0;

    for (int index = 0; index < num_rows; index++) {
        for (int index2 = 0; index2 < num_cols; index2++) {
            for (int size = 1; size + index <= num_rows && size + index2 <= num_cols; size++) {
                int valid = 1;

                for (int index3 = 0; index3 < size; index3++) {
                    if (bitmap[index][index2 + index3] != 1 || bitmap[index + size -1][index2 + index3] != 1) {
                        valid = 0;
                        break;
                    }
                }

                if (valid) {
                    for (int index3 = 0; index3 < size; index3++) {
                        if (bitmap[index + index3][index2] != 1 || bitmap[index + index3][index2 + size - 1] != 1) {
                            valid = 0;
                            break;
                        }
                    }
                }

                if (valid && (size > maximum_size || (size == maximum_size && (index < *start_x || (index == *start_x && index2 < *start_y))))) {
                    maximum_size = size;
                    *start_x = index;
                    *start_y = index2;
                    *end_x = index + size - 1;
                    *end_y = index2 + size - 1;
                }
            }
        }
    }

    return maximum_size;  
}

/**
 * Main process function to handle the operations.
 * @param filename Name of the text file.
 * @param operation Operation to perform (test, hline, vline, square, --help).
 * @param result Pointer to store the result of the operation.
 */
void process (const char *filename, const char *operation, int *result) {
    int num_rows, num_cols;
    int **bitmap;
    int start_x, start_y, end_x, end_y;

    // Check the validity of the file and read the bitmap.
    *result = validity_check (filename, &num_rows, &num_cols, &bitmap);
    if (*result != VALID) {
        return;
    } 

    if (strcmp (operation, "test") == 0) {
        *result = VALID;
        return;
    }

    if (strcmp (operation, "--help") == 0) {
        printf ("Usage: figsearch <operation> <filename>\n" 
                "Operations:\n"
                " --help Show this help message and exit.\n" 
                " test Validate the bitmap file format.\n" 
                " hline Find the longest horizontal line.\n" 
                " vline Find the longest vertical line.\n" 
                " square Find the largest square.\n" 
                "\n"
                "Description:\n" 
                " The figsearch program processes a bitmap file and performs various\n" 
                " operations to analyze the content.");

    } else if (strcmp (operation, "hline") == 0) {
        int hline_lenght = find_hline (num_rows, num_cols, bitmap, &start_x, &start_y, &end_x, &end_y);
        if (hline_lenght > 0) {
            printf ("%d %d %d %d\n", start_x, start_y, end_x, end_y);
        } else {
            printf ("No horizontal line found.\n");
        }

    } else if (strcmp (operation, "vline") == 0) {
        int vline_lenght = find_vline (num_rows, num_cols, bitmap, &start_x, &start_y, &end_x, &end_y);
        if (vline_lenght > 0) {
            printf ("%d %d %d %d\n", start_x, start_y, end_x, end_y);
        } else {
            printf ("No vertical line found.\n");
        }

    } else if (strcmp (operation, "square") == 0) {
        int square_lenght = find_square (num_rows, num_cols, bitmap, &start_x, &start_y, &end_x, &end_y);
        if (square_lenght > 0) {
            printf ("%d %d %d %d\n", start_x, start_y, end_x, end_y);
        } else {
            printf ("No square found.\n");
        }
    } else {
        printf ("Unknown operation.\n");
    }

    // Free allocated memory.
    for (int val = 0; val < num_rows; val++) {
            free (bitmap[val]);
    }

    free (bitmap);
}