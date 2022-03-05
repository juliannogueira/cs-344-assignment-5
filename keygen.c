#include "keygen.h"
#include "util.h"

/*
 * Write a key of size characters to stdout.
 *
 * The key consists of pseudorandom characters, uppercase letters and the space
 * character.
 * 
 * A newline character is the last character written to stdout.
 */
void create_key(int size) {
    int ch = 0;
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        ch = rand() % (27);
        if (ch == 26) {
            ch = 32;
        } else {
            ch += 65;
        }
        printf("%c", ch);
    }
    printf("\n");
}

/*
 * Create a key of size length.
 * 
 * Exactly one command line argument must be entered.
 *
 * The size is parsed from the command line argument.
 */
int main(int argc, char **argv) {
    if (argc == 2) {
        int size = string_to_integer(*(argv + 1));
        create_key(size);
    } else {
        fprintf(stderr, "Error: expected one argument\n");
    }
    return 0;
}