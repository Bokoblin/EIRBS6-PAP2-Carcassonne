#include <stdlib.h>
#include "board.h"
#include "../common/com_func_ptrs.h"
#include "../common/deck.h"
#include "../common/meeple.h"
#include "../common/utils.h"

struct board *board__init()
{
    struct board *b = safe_malloc(sizeof(struct board));

    b->cards_set = set__empty(card_copy_op, card_delete_op, card_compare_op, card_debug_op);
    b->meeples_set = set__empty(meeple_copy_op, meeple_delete_op, meeple_compare_op, meeple_debug_op);
    b->moves_queue = queue__empty(move_copy_op, move_delete_op, move_debug_op);
    b->drawing_stack = stack__empty(&cardid_copy_op, &cardid_delete_op, &cardid_debug_op);;
    //NOTE: Not filling it inside init at least for now because board is also used by clients
    b->first_card = NULL;

    return b;
}

int board__init_first_card(struct board *b)
{
    if (b == NULL)
        return !SUCCESS;

    if (stack__is_empty(b->drawing_stack)) {
        printf("[ERROR] : Card drawing stack is empty, please fill it before");
        return !SUCCESS;
    }

    enum card_id *ci = stack__pop(b->drawing_stack);
    struct card *c = card__init(*ci);
    free(ci);
    c->pos.x = 0;
    c->pos.y = 0;
    set__add(b->cards_set, c);
    b->first_card = set__get_umpteenth_no_copy(b->cards_set, 0);
    card__free(c);

    return SUCCESS;
}

int board__is_valid_card(struct board *b, enum card_id ci)
{
    if (b == NULL || ci == LAST_CARD)
        return false;

    struct card *c = card__init(ci);
    for (size_t i = 0; i < set__size(b->cards_set); i++) {
        if (card__are_matching_free_side(c, set__get_umpteenth_no_copy(b->cards_set, i))) {
            card__free(c);
            return true;
        }
    }
    card__free(c);
    return false;
}

int board__add_card(struct board *b, struct card *card_to_add)
{
    if (b == NULL || card_to_add == NULL)
        return !SUCCESS;

    if (set__retrieve(b->cards_set, card_to_add) != NULL) //a card of same position is already there
        return !SUCCESS;

    enum card_id ci = LAST_CARD;
    struct card *search_helper_card = card__init(ci);

    //=== Checking if the card to add can be linked to another already on the board

    int nb_possible_neighbours = 0;
    for (unsigned int i = 0; i < DIRECTION_NUMBER; i++) {
        search_helper_card->pos = card__get_position_at_direction(card_to_add, (enum direction) i);
        struct card *neighbour = (struct card *) set__retrieve(b->cards_set, search_helper_card);
        if (neighbour != NULL) {
            for (unsigned int j = 0; j < DIRECTION_NUMBER; j++) {
                if (card__are_matching_directions(card_to_add, neighbour, (enum direction) i, (enum direction) j)) {
                    nb_possible_neighbours++;
                    break;
                }
                else if (nb_possible_neighbours != 0) {
                    card__free(search_helper_card);
                    return !SUCCESS; //All or nothing match
                }
            }
        }
    }

    if (nb_possible_neighbours == 0) {
        card__free(search_helper_card);
        return !SUCCESS;
    }

    //=== Adding card to the board (to avoid linking copies)
    //TODO: Optimize! A possible solution could be to make a copy_operator correctly handling neighbours ptr linking

    if(set__add(b->cards_set, card_to_add) != SUCCESS) {
        card__free(search_helper_card);
        return !SUCCESS;
    }
    struct card* c_in_set = set__retrieve(b->cards_set, card_to_add);

    //=== Linking cards

    for (unsigned int i = 0; i < DIRECTION_NUMBER; i++) {
        enum direction d = (enum direction) i; //card's direction to link
        search_helper_card->pos = card__get_position_at_direction(card_to_add, d);
        struct card *neighbour = (struct card *) set__retrieve(b->cards_set, search_helper_card);
        if (neighbour != NULL) {
            for (unsigned int j = 0; j < DIRECTION_NUMBER; j++) {
                if (card__are_matching_directions(c_in_set, neighbour, d, (enum direction) j)) {
                    if (card__link_at_directions(c_in_set, neighbour, d, (enum direction) j) != SUCCESS) {
                        set__remove(b->cards_set, card_to_add);
                        card__free(search_helper_card);
                        return !SUCCESS;
                    }
                    else break;
                }
            }
        }
    }

    card__free(search_helper_card);

    return card__get_neighbour_number(c_in_set) == 0; //false -> SUCCESS
}

int board__add_meeple(struct board *b, struct meeple *m)
{
    if (b == NULL || m == NULL)
        return !SUCCESS;

    //TODO after May 4th: add meeple to board after verifications
    //Check if a meeple is already on the card
    //Graph path-course to check if meeple is authorized to be placed on requested area.
    (void) b;
    (void) m;

    //return set__add(b->meeples_set, m);
    return SUCCESS; //Meeple handling not needed for due date May 4th
}

void board__check_sub_completion(struct board *b)
{
    if (b == NULL) return;

    //TODO after May 4th: check if new areas were closed in order to calculate sub-score
    //-> mark already closed areas somewhere to avoid recalculation ?
    (void) b;
}

void board__free(struct board *b)
{
    if (b == NULL) return;

    //Never free first_card
    set__free(b->cards_set);
    set__free(b->meeples_set);
    queue__free(b->moves_queue);
    stack__free(b->drawing_stack);
    free(b);
}
