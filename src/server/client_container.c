#include "client_container.h"

struct client* init_client()
{
    struct client * c = malloc(sizeof(struct client));

    if (c == NULL) {
        exit_on_error("Malloc failure on: struct client *");
    } else {
        c->lib_ptr = NULL;
        c->get_player_name = NULL;
        c->initialize = NULL;
        c->play = NULL;
        c->finalize = NULL;
    }

    return c;
}

struct client_container* init_client_container(unsigned int clients_count)
{
    struct client_container *container = malloc(sizeof(struct client_container));
    if (container == NULL)
        exit_on_error("Malloc failure on: struct client_container *");
    else {
        container->current_size = 0;

        container->clients_array = malloc(clients_count * sizeof(struct client));
        if (container->clients_array == NULL)
            exit_on_error("Malloc failure on: struct client **");

        container->current_size = clients_count;

        for (unsigned int i=0; i < container->current_size; i++)
            container->clients_array[i] = init_client();


    }
    return container;
}

void free_client_container(struct client_container* container)
{
    if (container != NULL) {
        for (unsigned int i=0; i < container->current_size; i++)
            free(container->clients_array[i]);

        free(container->clients_array);
        free(container);
    }
}