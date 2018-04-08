#include "player_container.h"

struct player_container* init_player_container(unsigned int nb_player)
{
    struct player_container *container = malloc(sizeof(struct player_container));
    if (container == NULL)
        exit_on_error("Malloc failure on: struct player_container *");
    else {
        container->current_size = 0;

        container->player_array = malloc(nb_player * sizeof(struct player));
        if (container->player_array == NULL)
            exit_on_error("Malloc failure on: struct player **");

        container->current_size = nb_player;

        for (unsigned int i=0; i < container->current_size; i++)
            container->player_array[i] = init_player();

        container->current_player = rand() % container->current_size;
    }
    return container;
}

void free_player_container(struct player_container *container)
{
    if (container != NULL) {
        for (unsigned int i=0; i < container->current_size; i++)
            free_player(container->player_array[i]);

        free(container->player_array);
        free(container);
    }
}