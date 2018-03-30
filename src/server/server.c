#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include "../server/client_container.h"
#include "../common/projCarcassonneInterface.h"

#define DEFAULT_GRAPHIC_MODE_FLAG 0
#define DEFAULT_CLIENT_COUNT 0


/**
 * @brief Registers all client libraries passes in program arguments
 * @param argc argument count
 * @param argv argument content array
 * @param clients the clients container
 * @param clients_count the clients to register count
 */
void register_clients(int argc, const char **argv, const struct client_container *clients, unsigned int clients_count)
{
    unsigned int nb_clients_registered = 0;
    int args_index = 0;

    while (nb_clients_registered < clients_count && args_index < argc) {
        const char* current_arg = argv[args_index];
        if (strstr(current_arg, ".so") != NULL) {
            printf("Registering: %s\n", current_arg);
            clients->clients_array[nb_clients_registered]->lib_ptr = dlopen(current_arg, RTLD_NOW);
            assert_no_dlerror();
            clients->clients_array[nb_clients_registered]->get_player_name
                    = dlsym(clients->clients_array[nb_clients_registered]->lib_ptr, "get_player_name");
            assert_no_dlerror();
            clients->clients_array[nb_clients_registered]->initialize
                    = dlsym(clients->clients_array[nb_clients_registered]->lib_ptr, "initialize");
            assert_no_dlerror();
            clients->clients_array[nb_clients_registered]->play
                    = dlsym(clients->clients_array[nb_clients_registered]->lib_ptr, "play");
            assert_no_dlerror();
            clients->clients_array[nb_clients_registered]->finalize
                    = dlsym(clients->clients_array[nb_clients_registered]->lib_ptr, "finalize");
            assert_no_dlerror();

            nb_clients_registered++;
        }
        args_index++;
    }
}

struct move* previous_moves__empty(unsigned int client_count)
{
    struct move *previous_moves = malloc(client_count * sizeof(struct move));
    if (previous_moves == NULL)
        exit_on_error("Malloc failure on: struct client_container *");
    else {
        for (size_t i = 0; i < client_count; i++) {
            struct move move_placeholder = {FAILED, (unsigned int) i, LAST_CARD, { 3, 4 }, NORTH, NO_MEEPLE};
            previous_moves[i] = move_placeholder;
        }
    }

    return previous_moves;
}

void previous_moves__add(struct move *previous_moves, unsigned int array_size, struct move new_move)
{
    if (array_size > 0 ) {
        if (array_size == 1) {
            previous_moves[0] = new_move;
        } else {
            for (unsigned int i = 0; i < array_size-1; i++) {
                previous_moves[i] = previous_moves[i+1];
            }
            previous_moves[array_size - 1] = new_move;
        }
    }
}

int main(int argc, char** argv)
{
    unsigned int is_graphic = DEFAULT_GRAPHIC_MODE_FLAG;
    unsigned int clients_count = DEFAULT_CLIENT_COUNT;
    parse_opts(argc, argv, &is_graphic, &clients_count);


    //=== Register clients

    struct client_container *clients = init_client_container(clients_count);
    register_clients(argc, (const char **) argv, clients, clients_count);

    //=== Init players

    for (size_t i = 0; i < clients->current_size; i++) {
        if (clients->clients_array[i]->initialize != NULL)
            clients->clients_array[i]->initialize((unsigned int) i, clients_count);
    }

    //=== Display players name

    if (clients->current_size > 0)
        printf("Placeholder execution of \"get_player_name\" for all players:\n");
    for (size_t i = 0; i < clients->current_size; i++) {
        if (clients->clients_array[i]->get_player_name != NULL)
            printf("\t- CLIENT#%d: %s\n", (int) (i+1), clients->clients_array[i]->get_player_name());
    }

    //=== Make each player play once

    struct move *previous_moves = previous_moves__empty(clients->current_size);

    for (size_t i = 0; i < clients->current_size; i++) {
        if (clients->clients_array[i]->play != NULL) {
            struct move new_move = clients->clients_array[i]->play(CARD_PLAIN_CITY, previous_moves, 0);
            previous_moves__add(previous_moves, clients->current_size, new_move);
        }
    }

    free(previous_moves);

    //=== Finalize each player

    for (size_t i = 0; i < clients->current_size; i++) {
        if (clients->clients_array[i]->finalize != NULL)
            clients->clients_array[i]->finalize();
    }

    //=== Cleaning resources

    for (size_t i = 0; i < clients->current_size; i++) {
        if (clients->clients_array[i]->lib_ptr != NULL) {
            dlclose(clients->clients_array[i]->lib_ptr);
            assert_no_dlerror();
        }
    }

    free_client_container(clients);

    return EXIT_SUCCESS;
}
