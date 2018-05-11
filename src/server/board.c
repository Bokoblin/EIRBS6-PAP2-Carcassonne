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

    if (b->first_card == NULL)
        exit_on_error("Missing first card");

    return SUCCESS;
}

int board__is_valid_card(struct board *b, enum card_id ci)
{
    if (b == NULL || ci == LAST_CARD)
        return false;

    struct card *c = card__init(ci);
    for (size_t i = 0; i < set__size(b->cards_set); i++) {
        if (card__are_matching_free_direction(c, set__get_umpteenth_no_copy(b->cards_set, i))) {
            card__free(c);
            return true;
        }
    }
    card__free(c);
    return false;
}

struct card* board__retrieve_card_by_position(struct set *set, struct position pos)
{
    struct card card_to_search;
    card_to_search.pos = pos;
    return set__retrieve(set, &card_to_search);
}

int board__add_card(struct board *b, struct card *card_to_add)
{
    if (b == NULL || card_to_add == NULL)
        return !SUCCESS;

    if (board__retrieve_card_by_position(b->cards_set, card_to_add->pos) != NULL) //check if the position is free
        return !SUCCESS;

    if(set__add(b->cards_set, card_to_add) != SUCCESS) //added card to set, to avoid linking copies if applicable
        return !SUCCESS;

    struct card* card_to_link = board__retrieve_card_by_position(b->cards_set, card_to_add->pos);

    for (enum direction i_card_to_link_dir = NORTH; i_card_to_link_dir < NB_DIRECTIONS; i_card_to_link_dir++) {
        struct position neighbour_pos = card__get_position_at_direction(card_to_add, i_card_to_link_dir);
        struct card *neighbour = board__retrieve_card_by_position(b->cards_set, neighbour_pos);

        if (neighbour != NULL && card__get_neighbour_number(neighbour) < NB_DIRECTIONS) {
            enum direction i_umpteenth_dir = (i_card_to_link_dir + 2) % NB_DIRECTIONS;

            if (card__are_matching_directions(card_to_link, neighbour, i_card_to_link_dir, i_umpteenth_dir)) {
                if (!card__link_at_directions(card_to_link, neighbour, i_card_to_link_dir, i_umpteenth_dir)) {
                    set__remove(b->cards_set, card_to_link);
                    return !SUCCESS;
                }
            } else { //Full matching or nothing
                if (card__get_neighbour_number(card_to_link) != 0) {
                    card__unlink_neighbours(card_to_link);
                    set__remove(b->cards_set, card_to_link);
                    return !SUCCESS;
                }
            }
        }
    }

    if (card__get_neighbour_number(card_to_link) == 0) {
        set__remove(b->cards_set, card_to_link);
        return !SUCCESS;
    }

    return SUCCESS;
}

int board__add_meeple(struct board *b, struct card *c, struct meeple *m)
{
    if (b == NULL || m == NULL)
        return !SUCCESS;

    //TODO: Add meeple to board after check
    //Check if a meeple is already on the card
    //Graph path-course to check if meeple is authorized to be placed on requested area.
    (void) b;
    (void) c;
    (void) m;

    //return set__add(b->meeples_set, m);
    return SUCCESS; //Meeple handling not needed for due date May 4th
}

void board__check_sub_completion(struct board *b)
{
    if (b == NULL) return;

    //TODO: Check if new areas were closed in order to calculate sub-score
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
