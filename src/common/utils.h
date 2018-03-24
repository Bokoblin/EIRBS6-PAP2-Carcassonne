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
 *
 * @author David Renault (creator), Arthur
 * @date ?/11/17 - 24/03/18
 */
void parse_opts(int argc, char **argv, unsigned int *is_graphic, unsigned int *clients_count);


/**
 * Displays an error message and exits the application
 * @param message the error message to print
 *
 * @author Arthur
 * @date 10/11/17
 */
void exit_on_error(char* message);

#endif