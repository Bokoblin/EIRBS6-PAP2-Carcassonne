#include <stdlib.h>
#include "board.h"
#include "utils.h"

#define DEFAULT_FREE_POSITIONS_SIZE 8

struct board* board__empty()
{
    struct board *b = malloc(sizeof(struct board));
    if (b == NULL) {
        exit_on_error("Malloc failure on: struct board*");
    } else {
        b->free_positions_array = malloc(sizeof(struct card) * DEFAULT_FREE_POSITIONS_SIZE);
        if (b->free_positions_array == NULL)
            exit_on_error("Malloc failure on: struct card**");

        b->fp_capacity = DEFAULT_FREE_POSITIONS_SIZE;
        b->free_positions_array[0] = card__empty(CARD_JUNCTION_THREE);
        b->fp_size = 1;
        //NOTE: Baptiste : I thought the free_positions_array array
        // was the list of cards next to free spaces (extremity cards)
    }

    return b;
}

struct card *board__first_card(struct board *b)
{
    return b->free_positions_array[0];
}

int board__add_card(struct board *b, struct card *c)
{
    if (b == NULL || c == NULL)
        return -1;

    if (b->fp_size == b->fp_capacity) {
        b->free_positions_array = realloc(b->free_positions_array, 2 * b->fp_capacity);

        if (b->free_positions_array == NULL)
            return -1;
    }

    //TODO : other checks

    b->free_positions_array[b->fp_size] = c;
    b->fp_size++;

    return 0;
}

void board__free(struct board *b)
{
    for (unsigned int i = 0; i < b->fp_size; i++){
        card__free(b->free_positions_array[i]);
    }

    free(b->free_positions_array);
    free(b);
}
