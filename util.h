#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int raise_to_power(int operand, int power); 

int random_integer(int max);

int string_length(char *string);

int string_to_integer(char *string);

void get_user_input(char *prompt, char *buffer, int size);

void error(char *message, int status);

#endif