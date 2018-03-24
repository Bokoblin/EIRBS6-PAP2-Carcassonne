#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "../common/projCarcassonneInterface.h"

#define CLIENTS_LOCATION (install/client)

/**
 * @brief Error handling
 * @param error_message the error message to display
 */
void handle_error(char* error_message)
{
    fputs(error_message, stderr);
    exit(EXIT_FAILURE);
}

int main()
{
    void *handle;
    const char * (*get_client1_name)(void);
    char *error;

    //TODO: dynamic loading all in folder
    handle = dlopen("install/client/client1.so", RTLD_NOW);
    if (!handle) handle_error(dlerror()); //on dlopen error

    get_client1_name = dlsym(handle, "get_player_name");
    if ((error = dlerror()) != NULL) handle_error(error); //on symbol not found


    /* TEST ZONE !!*/
    char const *player_name = get_client1_name();
    printf("%s\n", player_name);

    dlclose(handle);

    return EXIT_SUCCESS;
}
