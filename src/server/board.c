#include <stdlib.h>
#include "board.h"
#include "../common/deck.h"
#include "../common/meeple.h"
#include "../common/utils.h"

struct board *board__init()
{
    struct board *b = safe_malloc(sizeof(struct board));

    b->cards_set = set__empty(card__copy_op, card__delete_op, card__compare_op, card__debug_op);
    b->meeples_set = set__empty(meeple_copy_op, meeple_delete_op, meeple_compare_op, meeple_debug_op);
    b->moves_queue = queue__empty(move__copy_op, move__delete_op, move__debug_op);
    b->drawing_stack = stack__empty(&card_id__copy_op, &card_id__delete_op, &card_id__debug_op);;
    b->first_card = NULL;

    return b;
}

int board__init_deck_and_first_card(struct board *b)
{
    if (b == NULL)
        return !SUCCESS;

    init_deck(b->drawing_stack);

    if (stack__is_empty(b->drawing_stack)) {
        printf("[ERROR] : Card drawing stack is empty, please fill it before");
        return !SUCCESS;
    }

    enum card_id *ci = stack__pop(b->drawing_stack);
    int res = board__add_custom_first_card(b, *ci, (struct position){ 0, 0}, NORTH);
    card_id__delete_op(ci);

    return res;
}

int board__add_custom_first_card(struct board *b, enum card_id ci, struct position pos, enum direction north_dir)
{
    if (b == NULL)
        return !SUCCESS;

    struct card* c = card__init(ci);
    c->pos = pos;
    c->direction = north_dir;
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
        struct position neighbour_pos = card__get_position_at_direction(card_to_link, i_card_to_link_dir);
        struct card *neighbour = board__retrieve_card_by_position(b->cards_set, neighbour_pos);

        if (neighbour != NULL && card__get_neighbour_number(neighbour) < NB_DIRECTIONS) {
            enum direction i_umpteenth_dir = (i_card_to_link_dir + 2) % NB_DIRECTIONS;

            if (card__are_matching_directions(card_to_link, neighbour, i_card_to_link_dir, i_umpteenth_dir)) {
                if (!card__link_at_directions(card_to_link, neighbour, i_card_to_link_dir, i_umpteenth_dir)) {
                    set__remove(b->cards_set, card_to_link);
                    return !SUCCESS;
                }
            } else {
                card__unlink_neighbours(card_to_link);
                set__remove(b->cards_set, card_to_link);
                return !SUCCESS;
            }
        }
    }

    if (card__get_neighbour_number(card_to_link) == 0) {
        set__remove(b->cards_set, card_to_link);
        return !SUCCESS;
    }

    return SUCCESS;
}

int board__add_meeple(struct board *b, struct meeple *m)
{
    if (b == NULL || m == NULL)
        return !SUCCESS;

    //TODO: Add meeple to board after check
    (void) b;
    (void) m;

    return SUCCESS;
}

void board__check_sub_completion(struct board *b)
{
    if (b == NULL) return;

    //TODO: Check if new areas were closed in order to calculate sub-score
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
