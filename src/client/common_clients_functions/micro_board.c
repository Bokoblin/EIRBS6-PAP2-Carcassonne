#include "micro_board.h"
#include "../../common/utils.h"

struct micro_board *micro_board__init()
{
    struct micro_board *b = safe_malloc(sizeof(struct micro_board));

    b->cards_set = set__empty(card__copy_op, card__delete_op, card__compare_op, card__debug_op);

    struct card *c = card__init(FIRST_CARD_ID);
    c->pos.x = 0; c->pos.y = 0;
    set__add(b->cards_set, c);
    b->first_card = set__get_umpteenth_no_copy(b->cards_set, 0);
    card__free(c);

    if (b->first_card == NULL)
        exit_on_error("Missing first card");

    return b;
}

struct card* micro_board__retrieve_card_by_position(struct set *set, struct position pos)
{
    struct card card_to_search;
    card_to_search.pos = pos;
    return set__retrieve(set, &card_to_search);
}

int micro_board__is_card_adding_possible(struct micro_board *b, struct card *card)
{
    if (b == NULL || card == NULL)
        return false;

    if (micro_board__retrieve_card_by_position(b->cards_set, card->pos) != NULL) //check if the position is free
        return false;

    int neighbour_count = 0;

    for (enum direction i_card_dir = NORTH; i_card_dir < NB_DIRECTIONS; i_card_dir++) {
        struct position neighbour_pos = card__get_position_at_direction(card, i_card_dir);
        struct card *neighbour = micro_board__retrieve_card_by_position(b->cards_set, neighbour_pos);

        if (neighbour != NULL && card__get_neighbour_number(neighbour) < NB_DIRECTIONS) {
            enum direction i_neighbour_dir = (i_card_dir + 2) % NB_DIRECTIONS;
            if (card__are_matching_directions(card, neighbour, i_card_dir, i_neighbour_dir))
                neighbour_count++;
            else
                return false;
        }
    }

    return neighbour_count > 0;
}

int micro_board__add_card_to_board(struct micro_board *b, struct card *card)
{
    if(set__add(b->cards_set, card) != SUCCESS) //added card to set, to avoid linking copies
        return !SUCCESS;

    struct card* card_to_link = micro_board__retrieve_card_by_position(b->cards_set, card->pos);

    for (enum direction i_card_dir = NORTH; i_card_dir < NB_DIRECTIONS; i_card_dir++) {
        struct position neighbour_pos = card__get_position_at_direction(card, i_card_dir);
        struct card *neighbour = micro_board__retrieve_card_by_position(b->cards_set, neighbour_pos);

        if (neighbour != NULL && card__get_neighbour_number(neighbour) < NB_DIRECTIONS) {
            enum direction i_neighbour_dir = (i_card_dir + 2) % NB_DIRECTIONS;
            if (card__are_matching_directions(card_to_link, neighbour, i_card_dir, i_neighbour_dir)) {
                if (!card__link_at_directions(card_to_link, neighbour, i_card_dir, i_neighbour_dir)) {
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

void micro_board__free(struct micro_board *b)
{
    if (b == NULL) return;

    set__free(b->cards_set);
    free(b);
}
