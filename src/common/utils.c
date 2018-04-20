#include <string.h>
#include <dlfcn.h>
#include "utils.h"

void parse_opts(int argc, char **argv, unsigned int *is_graphic, unsigned int *clients_count)
{
    int opt;
    while ((opt = getopt(argc, argv, ":g:h:")) != -1) {
        switch (opt) {
            case 'g':
                *is_graphic = 1;
                break;
            case 'h':
                printf("Usage: ./install/server [-g] ./install/*.so\n");
                exit(EXIT_SUCCESS);
            default:
                break;
        }
    }

    for (int i = 0; i < argc; i++)
        if (strstr(argv[i], ".so") != NULL)
            (*clients_count)++;
}

void assert_no_dlerror()
{
    char *error = dlerror();
    if (error != NULL)
        exit_on_error(error);
}

void assert_not_null(const void *ptr, const char *caller_fct, char *object)
{
    if (ptr == NULL) {
        const size_t size = strlen(caller_fct) + strlen(": NULL value on ") + strlen(object) + 1;
        char message[size];
        snprintf(message, size, "%s: NULL value on %s", caller_fct, object);
        exit_on_error(message);
    }
}

void exit_on_error(char *message)
{
    printf("[ERROR] %s. Now exiting...\n", message);
    exit(EXIT_FAILURE);
}

size_t positive_modulo(int a, int b)
{
    if (b <= 0)
        return 0;
    if (a >= 0)
        return (size_t) (a%b);
    return (size_t) ((-a)%b);
}

void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
        exit_on_error("Memory allocation failure");
    return ptr;
}
