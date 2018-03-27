#include "client_container.h"
#include "../common/utils.h"

struct client_container* init_client_container(unsigned int clients_count)
{
    struct client_container *clients = malloc(sizeof(struct client_container));
    clients->current_size = 0;
    clients->max_size = 0;

    if (clients_count == 0)
        return clients;

    clients->clients_files_array = malloc(clients_count * sizeof(char*));
    if (clients->clients_files_array == NULL)
        exit_on_error("Malloc failure on clients_files_array void**");

    clients->clients_pointer_array = malloc(clients_count * sizeof(void*));
    if (clients->clients_pointer_array == NULL)
        exit_on_error("Malloc failure on clients_pointer_array void**");

    clients->max_size = clients_count;

    return clients;
}

void free_client_container(struct client_container* container)
{
    free(container->clients_files_array);
    free(container->clients_pointer_array);
    free(container);
}