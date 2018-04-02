#include <stdlib.h>
#include "board.h"
#include "utils.h"

#define DEFAULT_FREE_POSITIONS_SIZE 8

struct board* init_board()
{
    struct board *b = malloc(sizeof(struct board));
    if (b == NULL) {
        exit_on_error("Malloc failure on: struct board*");
    } else {
        b->first_card = card__empty(CARD_JUNCTION_THREE);
        b->fp_capacity = DEFAULT_FREE_POSITIONS_SIZE;
        b->free_positions = malloc(sizeof(struct card) * b->fp_capacity);
        b->free_positions[0] = b->first_card;
        //NOTE: Baptiste : I though the free_positions array was the list of cards next to free spaces (extremity cards)
    }

    return b;
}

void free_board(struct board* game)
{
    for (unsigned int i = 0; i < game->fp_capacity; i++){
        card__free(game->free_positions[i]);
    }

    if (game->first_card != NULL)
        free(game->first_card);
    free(game->free_positions);
    free(game);
}
