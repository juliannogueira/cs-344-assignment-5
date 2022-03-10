#include "util.h"

/*
 * Write an error message to stderr, and exit with the specified status.
 */
void error(char *message, int status) {
    fprintf(stderr,"%s", message);
    if (status >= 0) {
        exit(status);
    }
}

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
 * Convert an integer to a null-terminated character array.
 *
 * Update the dynamically allocated character array.
 */
void integer_to_string(int num, char *string) {
    int index = 0;
    int is_negative = 0;
    char temp = 0;

    if (num < 0) {
        is_negative = 1;
        num *= -1;
    }

    while (num != 0) {
        index++;
        *(string + index - 1) = (num % 10) + 48;
        num /= 10;
    }

    if (is_negative) {
        index++;
        *(string + index - 1) = '-';
    }

    for (int i = 0; i < index / 2; i++) {
        temp = *(string + i);
        *(string + i) = *(string + index - 1 - i);
        *(string + index - 1 - i) = temp;
    }

    index++;
    *(string + index - 1) = '\0';
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
 * Reallocate a buffer of a specified length to hold count extra characters.
 *
 * The string is null-terminated.
 */
char *reallocate_string(char *buffer, int length, int count) {
    char temp[length];
    for (int i = 0; i < length; i++) {
        temp[i] = *(buffer + i);
    }
    buffer = realloc(buffer, sizeof(char) * (length + count));
    for (int i = 0; i < length; i++) {
        *(buffer + i) = temp[i];
    }
    *(buffer + length) = '\0';
    return buffer;
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
 * Send data to a socket.
 */
void send_data(int socket, char *buffer, int count) {
    int is_sending = 1;
    int index = 0;

    while (is_sending) {
        index += send(socket, buffer + index, count - index, 0);

        if (index == count) {
            is_sending = 0;
        } else if (index < 0) {
            error("Error: could not write to socket\n", 2);
        }
    }
}

/*
 * Receive data from a socket.
 */
void receive_data(int socket, char *buffer, int count) {
    int is_receiving = 1;
    int index = 0;

    while (is_receiving) {
        index += recv(socket, buffer + index, count - index, 0);

        if (index == count) {
            *(buffer + index) = '\0';
            is_receiving = 0;
        } else if (index < 0) {
            error("Error: could not read from socket\n", 2);
        }
    }
}