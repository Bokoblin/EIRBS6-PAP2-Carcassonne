#ifndef UTILS_H
#define UTILS_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Function for parsing the options of the program
 * Available options are :
 * -g "graphic" : to enable graphic mode (unsupported yet)
 * -h        : displays the help
 * @param argc the number of argument
 * @param argv  the list of arguments
 * @param is_graphic to enable graphic mode
 * @param clients_count number of clients passed in program arguments to count
 */
void parse_opts(int argc, char **argv, unsigned int *is_graphic, unsigned int *clients_count);


/**
 * @brief Checks that no dlerror has occurred
 */
void assert_no_dlerror();


/**
 * Displays an error message and exits the application
 * @param message the error message to print
 */
void exit_on_error(char* message);


/**
 * @bried Function calculating a positive modulo between two values
 * @param a a value
 * @param b a value
 * @return a modulo
 */
unsigned int positive_modulo(int a, int b);

#endif