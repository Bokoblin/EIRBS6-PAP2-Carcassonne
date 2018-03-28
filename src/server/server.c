#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include "../common/projCarcassonneInterface.h"
#include "../common/utils.h"
#include "../server/client_container.h"

#define DEFAULT_GRAPHIC_MODE_FLAG 0
#define DEFAULT_CLIENT_COUNT 0


int main(int argc, char** argv)
{
    unsigned int is_graphic = DEFAULT_GRAPHIC_MODE_FLAG;
    unsigned int clients_count = DEFAULT_CLIENT_COUNT;
    parse_opts(argc, argv, &is_graphic, &clients_count);


    //=== init clients

    struct client_container *clients = init_client_container(clients_count);

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

    const char * (*initialize)(unsigned int, unsigned int);

    for (size_t i = 0; i < clients->current_size; i++) {
        initialize = dlsym(clients->clients_pointer_array[i], "initialize");
        assert_no_dlerror();
        initialize((unsigned int) i, clients_count);
    }


    //=== Display clients name

    const char * (*get_player_name)(void);

    printf("Placeholder execution of \"get_player_name\" for all players:\n");

    for (size_t i = 0; i < clients->current_size; i++) {
        get_player_name = dlsym(clients->clients_pointer_array[i], "get_player_name");
        assert_no_dlerror();
        printf("CLIENT#%d: %s\n", (int) (i+1), get_player_name());
    }


    //=== Make each client play once

    struct move previous_moves[] = {};

    struct move (*play)(enum card_id, struct move const[], size_t);

    for (size_t i = 0; i < clients->current_size; i++) {
        play = dlsym(clients->clients_pointer_array[i], "play");
        assert_no_dlerror();
        struct move current_move = play(CARD_PLAIN_CITY, previous_moves, 0);
    }


    //=== Finalize each client

    void (*finalize)(void);

    for (size_t i = 0; i < clients->current_size; i++) {
        finalize = dlsym(clients->clients_pointer_array[i], "finalize");
        assert_no_dlerror();
        finalize();
    }


    //=== Cleaning resources

    for (size_t i = 0; i < clients->current_size; i++) {
        dlclose(clients->clients_pointer_array[i]);
    }

    free_client_container(clients);

    return EXIT_SUCCESS;
}
