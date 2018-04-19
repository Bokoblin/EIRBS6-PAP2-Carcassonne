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
        struct card *c = card__init(*ci);
        free(ci);
        c->pos.x = 0;
        c->pos.y = 0;
        set__add(b->cards_set, c);
        b->first_card = set__get_i_th_no_copy(b->cards_set, 0);
        card__free(c);
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
        enum direction d = (enum direction) i; //c's direction to link
        search_helper_card.pos = p_array[i]; //Supposed position of searched card following direction chosen
        struct card *neighbour = (struct card *) set__retrieve(b->cards_set, &search_helper_card);
        if (neighbour != NULL && card__are_matching_direction(c, neighbour, d))
                card__link_at_direction(c, neighbour, d);
    }

    if (card__get_neighbour_number(c) == 0)
        return !SUCCESS; //if unbound card

    return set__add(b->cards_set, c);
}

void board__free(struct board *b)
{
    //Never free first_card
    set__free(b->cards_set);
    set__free(b->meeples_set);
    free(b);
}
