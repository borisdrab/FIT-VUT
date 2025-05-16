#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_STRING_LENGHT 102
#define MAX_NAME 50 
#define MAX_ARG_LENGHT 250

// A structure for the contact containing the name, converted name, and phone number.
typedef struct {
    char name[MAX_STRING_LENGHT];
    char name2[MAX_STRING_LENGHT];
    char tel_num[MAX_STRING_LENGHT];
} Contact;

/**
 * Converts name characters to numbers according to the telephone keypad.
 * @param name Original name.
 * @param converted_name Converted name.
 */
void convert_name_to_numbers(char name[MAX_STRING_LENGHT], char converted_name[MAX_STRING_LENGHT]){
    for (int i = 0; i < MAX_STRING_LENGHT && name[i] != '\0'; i++) {
        name[i] = tolower(name[i]);
        switch (name[i]) {
            case '+': converted_name[i] = '0';break;
            case 'a': case 'b': case 'c': converted_name[i] = '2'; break;
            case 'd': case 'e': case 'f': converted_name[i] = '3'; break;
            case 'g': case 'h': case 'i': converted_name[i] = '4'; break;
            case 'j': case 'k': case 'l': converted_name[i] = '5'; break;
            case 'm': case 'n': case 'o': converted_name[i] = '6'; break;
            case 'p': case 'q': case 'r': case 's': converted_name[i] = '7'; break;
            case 't': case 'u': case 'v': converted_name[i] = '8'; break;
            case 'w': case 'x': case 'y': case 'z': converted_name[i] = '9'; break;
            case '\n': converted_name[i] = '\0'; name[i] = '\0'; break;
            default: break;
        }
        converted_name[i + 1] = '\0';
    }
}

/**
 * Converts a leading zero to a '+'.
 * @param query The original query.
 */
void convert_query(char *query) {
    if (query[0] == '0') {
        query[0] = '+';
    }
}

/** 
 * Checks if a substring is a part of a string.
 * @param string The main string.
 * @param sub_string The substring.
 * @return true, if the substring is in the string, false otherwise.
 */
bool is_substring(char string[],char sub_string[]) {
    int len = strlen (sub_string);
    if (len == 0) return true;
    int first_index = 0;
    for (int second_index = 0; second_index < MAX_STRING_LENGHT; second_index++){    
            for (first_index = 0; first_index < len + 1; first_index++){
                if (len == first_index) {
                    return true;
                }
                if (sub_string[first_index] != string[second_index + first_index]) {
                    break;
                }
            }
    }
    return false;
}

/**
 * Checks if the substring is in the phone number, with the option to convert the first character.
 * @param string The phone number.
 * @param sub_string The substring.
 * @return true, if the substring is in a phone number, false otherwise.
 */
bool is_substring_tel(char string[],char sub_string[]) {
    bool is_sub = false;
    if (string[0] == '+' && sub_string[0] == '0') {
        string[0] = '0';
        is_sub = is_substring(string,sub_string);
        string[0] = '+';
    } else {
        is_sub = is_substring(string,sub_string);
    }
    return is_sub;
}

/**
 * Checks whether the string is a valid numeric string.
 * @param str String to check.
 * @return 1, if the string is valid, 0 otherwise.
 */
int is_valid_number(const char *str) {
    while (*str) {
        if (!isdigit(*str) && *str != '+') {
            return 0;
        }
        str++;
    }
    return 1;
}

/**
 * The main function of the program.
 */
int main(int argc, char *argv[]) {
    char arg[MAX_STRING_LENGHT] = {0};
    // Check command line arguments.
    if (argc == 2) {
        if (!is_valid_number(argv[1])) {
            fprintf(stderr, "Error: Invalid characters. Use only digits and '+'.\n");
            return 1;
        }
        strncpy(arg, argv[1], strlen(argv[1])+ 1);

    } else if (argc == 1) {
        arg[0] = '\0';
    } else {
        fprintf(stderr, "Usage %s <query>\n", argv[0]);
        return 1;
    }

    Contact contacts;
    int contact_count = 0;

    // Reading contacts from input.
    while (fgets(contacts.name2, MAX_STRING_LENGHT, stdin) != NULL) {
        if (contacts.name2[strlen(contacts.name2) - 1] != '\n'){
            fprintf (stderr, "Error: Name exceeds maximum lenght of 100 characters.\n");
            return 1;
        }
        if (fgets(contacts.tel_num, MAX_STRING_LENGHT, stdin) == NULL) {
            fprintf(stderr, "Error reading telephone number.\n");
            return 1;
        }
        if (strlen(contacts.tel_num) >= MAX_STRING_LENGHT - 1 && contacts.name2[100] != '\n') {
            fprintf(stderr, "Error: Phone number exceeds maximum length of 100 characters.\n");
            return 1;
        }
        
        // Convert name to numbers.
        convert_name_to_numbers (contacts.name2,contacts.name);

        // Checking whether the query corresponds to a name or a phone number.
        if (is_substring(contacts.name,arg) || is_substring_tel(contacts.tel_num,arg)){
            contact_count++;
            printf("%s, %s",contacts.name2,contacts.tel_num);
        }

    }

    // If no contacts are found.
    if (contact_count == 0){
        printf("Not found\n");
    }

    return 0;
}