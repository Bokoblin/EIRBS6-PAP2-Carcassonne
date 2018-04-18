#ifndef UTILS_H
#define UTILS_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define true 1
#define false 0

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
 * @brief Checks that no dlerror has occurred, quit otherwise
 */
void assert_no_dlerror();


/**
 * @brief Checks that a pointer isn't null, quit otherwise
 */
void assert_not_null(void *ptr, const char *caller_fct, char *object);


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
size_t positive_modulo(int a, int b);


/**
 * @brief Perform a safe malloc interrupting program on memory allocation failure
 * @param size the size to allocate
 * @return a general pointer
 */
void* safe_malloc(size_t size);


/**
 * Execute safely dlsym by quitting on error
 * @param library_ptr the library pointer
 * @param target_ptr the target pointer
 * @param function_name the function name
 */
void safe_dlsym(void *library_ptr, void *target_ptr, char* function_name);

#endif