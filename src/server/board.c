#include <stdlib.h>
#include "board.h"
#include "../common/utils.h"
#include "../common/ADT/set.h"
#include "../server/meeple.h"
#include "../common/deck.h"
#include "player.h"
#include "function_pointers.h"

struct board *board__init(struct stack *drawing_stack)
{
    struct board *b = safe_malloc(sizeof(struct board));

    b->cards_set = set__empty(card_copy_op, card_delete_op, card_compare_op);
    b->meeples_set = set__empty(meeple_copy_op, meeple_delete_op, meeple_compare_op);

    enum card_id *ci = stack__pop(drawing_stack);
    if (ci == NULL) {
        b->first_card = NULL;
    } else {
        b->first_card = card__init(*ci);
        b->first_card->pos.x = 0;
        b->first_card->pos.y = 0;
        free(ci);
        set__add(b->cards_set, b->first_card);
    }

    return b;
}

int board__add_card(struct board *b, struct card *c)
{
    if (b == NULL || c == NULL)
        return !SUCCESS;

    struct card search_helper_card;
    struct position p_array[4]  = {
            //We consider North > South and East > West
            { c->pos.x, c->pos.y + 1 }, //North
            { c->pos.x - 1, c->pos.y }, //West
            { c->pos.x, c->pos.y - 1 }, //South
            { c->pos.x + 1, c->pos.y }  //East
    };

    for (unsigned int i = 0; i < 4; i++) {
        //enum direction d = (enum direction) ((i + 2) % DIRECTION_NUMBER); //c position in comparison to searched card
        search_helper_card.pos = p_array[i]; //Supposed position of searched card
        struct card *neighbour = (struct card *) set__retrieve(b->cards_set, &search_helper_card);
        if (neighbour != NULL && card__are_matching_direction(c, neighbour, (enum direction) i))
                card__link_at_direction(c, neighbour, (enum direction) i);
    }

    if (card__get_neighbour_number(c) == 0)
        return !SUCCESS; //if unbound card

    return set__add(b->cards_set, c);
}

void board__free(struct board *b)
{
    if (b->first_card != NULL)
        card__free(b->first_card);
    set__free(b->cards_set);
    set__free(b->meeples_set);
    free(b);
}
