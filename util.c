#include "util.h"

/*
 * Raise an operand to a specified power.
 */
int raise_to_power(int operand, int power) {
    int result = 0;
    for (int i = 0; i < power + 1; i++) {
        if (i == 0) {
            result = 1;
        } else {
            result *= operand;
        }
    }
    return result;
}

/*
 * Get a random integer.
 */
int random_integer(int max) {
    int result;
    time_t t;
    srand((unsigned) time(&t));
    srand(time(NULL));
    result = rand() % (max + 1);
    return result;
}

/*
 * Get the size of a null-terminated character array.
 */
int string_length(char *str) {
    int index = 0;
    while (*(str + index) != '\0') {
        index++;
    }
    return index;
}

/*
 * Convert a string to an integer.
 *
 * Negative integers are handled.
 */
int string_to_integer(char *string) {
    int result = 0;
    int is_digit = 1;
    int is_negative = 0;
    int ch = -1;
    int position = 0;
    for (int i = string_length(string) - 1; i > -1 ; i--) {
        ch = *(string + i);
        if (i == 0 && ch == 45) {
            is_negative = 1;
        } else if (ch >= 48 && ch <= 57) {
            ch -= 48;
            result += ch * raise_to_power(10, position);
            position += 1;
        } else {
            result = 0;
            is_digit = 0;
            break;
        }
    }
    if (is_digit && is_negative) {
        result *= -1;
    }
    return result;
}

/*
 * Prompt the user for input.
 *
 * Display the prompt, then get size characters from the user.
 * 
 * The newline character or last character, whichever comes first, will be
 * converted to a null-terminator.
 */
void get_user_input(char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    fflush(stdout);
    fgets(buffer, size, stdin);
    for (int i = 0; i < size; i++) {
        if (*(buffer + i) == '\n' || i == size - 1) {
            *(buffer + i) = '\0';
            break;
        }
    }
}

/*
 * Write an error message to stderr, and exit with the specified status.
 */
void error(char *message, int status) {
    fprintf(stderr,"%s", message);
    if (status >= 0) {
        exit(status);
    }
}
