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
    char *error;
    if ((error = dlerror()) != NULL)
        exit_on_error(error);
}

void exit_on_error(char *message)
{
    fputs("[ERROR] ", stderr);
    fputs(message, stderr);
    fputs(". Now exiting...\n", stderr);
    exit(EXIT_FAILURE);
}

unsigned int positive_modulo(int a, int b)
{
    if (b <= 0)
        return 0;

    while (a < 0)
        a += b;

    return (unsigned int) (a%b);
}
