#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include "../common/projCarcassonneInterface.h"
#include "../common/utils.h"
#include "client_container.h"

#define DEFAULT_GRAPHIC_MODE_FLAG 0
#define DEFAULT_CLIENT_COUNT 0

void initialize(unsigned int id, unsigned int n_players)
{
    //TODO
}

void finalize()
{
    //TODO
}

int main(int argc, char** argv)
{
    unsigned int is_graphic = DEFAULT_GRAPHIC_MODE_FLAG;
    unsigned int clients_count = DEFAULT_CLIENT_COUNT;
    parse_opts(argc, argv, &is_graphic, &clients_count);


    //=== init clients

    struct clients *clients = init_client_container(clients_count);

    for (int i = 0; i < argc; i++) {
        if (strstr(argv[i], "./install/client/") != NULL && clients->current_size < clients->max_size) {
            printf("Registering: %s\n", argv[i]);
            clients->clients_files_array[clients->current_size] = argv[i];
            clients->current_size++;
        }
    }

    for (size_t i = 0; i < clients->current_size; i++) {
        clients->clients_pointer_array[i] = dlopen(clients->clients_files_array[i], RTLD_NOW);
        if (!clients->clients_pointer_array[i])
            exit_on_error(dlerror()); //on dlopen error
    }


    //=== TEST ZONE

    const char * (*get_client1_name)(void);
    char *error;

    printf("The following clients were registered:\n");
    for (size_t i = 0; i < clients->current_size; i++) {
        get_client1_name = dlsym(clients->clients_pointer_array[i], "get_player_name");

        if ((error = dlerror()) != NULL)
            exit_on_error(error); //on symbol not found

        char const *player_name = get_client1_name();
        printf("CLIENT#%d: %s\n", (int) (i+1), player_name);
    }


    //=== QUITTING

    for (size_t i = 0; i < clients->current_size; i++) {
        dlclose(clients->clients_pointer_array[i]);
    }

    free_client_container(clients);

    return EXIT_SUCCESS;
}
