#include "server.h"
#include <stdlib.h>
#include "board.h"
#include "../common/utils.h"
#include "../common/ADT/set.h"
#include "../common/meeple.h"
#include "../common/deck.h"
#include "player.h"
#include "function_pointers.h"

struct board *board__init(struct stack *drawing_stack)
{
    struct board *b = safe_malloc(sizeof(struct board));

    b->cards_set = set__empty(card_copy_op, card_delete_op, card_compare_op, card_debug_op);
    b->meeples_set = set__empty(meeple_copy_op, meeple_delete_op, meeple_compare_op, meeple_debug_op);
    b->moves_stack = stack__empty(move_copy_op, move_delete_op, move_debug_op);

    if (drawing_stack == NULL) {
        b->first_card = NULL;
    } else {
        enum card_id *ci = stack__pop(drawing_stack);
        struct card *c = card__init(*ci);
        free(ci);
        c->pos.x = 0;
        c->pos.y = 0;
        set__add(b->cards_set, c);
        b->first_card = set__get_umpteenth_no_copy(b->cards_set, 0);
        card__free(c);
    }

    return b;
}

int board__is_valid_card(struct board *b, enum card_id ci)
{
    struct card *c = card__init(ci);
    for (size_t i = 0; i < set__size(b->cards_set); i++) {
        if (card__are_matching(c, set__get_umpteenth_no_copy(b->cards_set, i))) {
            card__free(c);
            return true;
        }
    }
    card__free(c);
    return false;
}

int board__add_card(struct board *b, struct card *c)
{
    if (b == NULL || c == NULL)
        return !SUCCESS;

    if (set__retrieve(b->cards_set, c)) //a card of same position is already there
        return !SUCCESS;

    enum card_id ci = LAST_CARD;
    struct card *search_helper_card = card__init(ci);
    struct position p_array[4]  = {
            //We consider North > South and East > West
            { c->pos.x, c->pos.y + 1 }, //North
            { c->pos.x - 1, c->pos.y }, //West
            { c->pos.x, c->pos.y - 1 }, //South
            { c->pos.x + 1, c->pos.y }  //East
    };

    for (unsigned int i = 0; i < 4; i++) {
        enum direction d = (enum direction) i; //c's direction to link
        search_helper_card->pos = p_array[i]; //Supposed position of searched card following direction chosen
        struct card *neighbour = (struct card *) set__retrieve(b->cards_set, search_helper_card);
        if (neighbour != NULL && card__are_matching_direction(c, neighbour, d))
            if (card__link_at_direction(c, neighbour, d) != SUCCESS)
                return !SUCCESS;
    }
    card__free(search_helper_card);

    if (card__get_neighbour_number(c) == 0)
        return !SUCCESS; //if unbound card

    return set__add(b->cards_set, c);
}

int board__add_meeple(struct board *b, struct meeple *m)
{
    //TODO: add meeple to board after verifications
    //Check if a meeple is already on the card
    //Graph path-course to check if meeple is authorized to be placed on requested area.
    (void) b;
    (void) m;

    //return set__add(b->meeples_set, m);
    return !SUCCESS;
}

void board__check_sub_completion(struct board *b)
{
    //TODO: check if new areas were closed in order to calculate sub-score
    //-> mark already closed areas somewhere to avoid recalculation ?
    (void) b;
}

void board__free(struct board *b)
{
    //Never free first_card
    set__free(b->cards_set);
    set__free(b->meeples_set);
    stack__free(b->moves_stack);
    free(b);
}
