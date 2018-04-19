#include <stdlib.h>
#include "test_utils.h"
#include "../src/server/board.h"
#include "../src/common/ADT/set.h"
#include "../src/common/utils.h"

int test_board__empty()
{
    printf("%s... ", __func__);

    struct board* b1 = board__init(NULL);

    if (b1->first_card != NULL || !set__is_empty(b1->cards_set) || !set__is_empty(b1->meeples_set)) {
        board__free(b1);
        return !TEST_SUCCESS;
    }

    board__free(b1);

    struct stack* s = stack__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id ci_first = CARD_ROAD_STRAIGHT_CITY;
    stack__push(s, &ci_first);
    b1 = board__init(s);
    b1->first_card->orientation = NORTH_TO_EAST;

    if (b1->first_card == NULL || set__is_empty(b1->cards_set) || !set__is_empty(b1->meeples_set)) {
        board__free(b1);
        stack__free(s);
        return !TEST_SUCCESS;
    }

    board__free(b1);
    stack__free(s);

    return TEST_SUCCESS;
}

int test_board__add_card()
{
    printf("%s... ", __func__);

    //First test: adding in empty set

    struct board *b = board__init(NULL);
    struct card *c1 = card__init(CARD_MONASTERY_ROAD);
    c1->pos.x = 0;
    c1->pos.y = 1;

    if (board__add_card(b, c1) == SUCCESS || card__get_neighbour_number(c1) != 0) {
        board__free(b);
        card__free(c1);
        return !TEST_SUCCESS;
    }

    board__free(b);

    //Second test: adding in non empty set (1) with no match

    struct stack* s = stack__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id ci_first = CARD_ROAD_STRAIGHT_CITY;
    stack__push(s, &ci_first);
    b = board__init(s);
    b->first_card->orientation = NORTH_TO_EAST;

    if (board__add_card(b, c1) == SUCCESS || card__get_neighbour_number(c1) != 0) {
        board__free(b);
        stack__free(s);
        card__free(c1);
        return !TEST_SUCCESS;
    }

    //Third test: adding in non empty set with a match

    c1->type = card__id_to_type(CARD_CITY_TUNNEL);

    if (board__add_card(b, c1) != SUCCESS || card__get_neighbour_number(c1) != 1
            || c1->neighbors[SOUTH] != b->first_card) {
        board__free(b);
        stack__free(s);
        card__free(c1);
        return !TEST_SUCCESS;
    }

    //Fourth test: adding until having a full surrounding for CARD_CITY_TUNNEL

    int res = !EXIT_SUCCESS;
    //FIXME: orientation

    struct card *c2 = card__init(CARD_CITY_ALL_SIDES);
    c2->pos.x = -1;
    c2->pos.y = 0;
    if (board__add_card(b, c2) != SUCCESS) { //must fail
        c2->pos.x = 0;
        c2->pos.y = 2;
        c2->orientation = NORTH_TO_NORTH;
        if (board__add_card(b, c2) == SUCCESS) { //must pass like beyond ones
            struct card *c3 = card__init(CARD_ROAD_TURN_RIGHT_CITY);
            c3->pos.x = -1;
            c3->pos.y = 2;
            c3->orientation = NORTH_TO_EAST;
            if (board__add_card(b, c3) == SUCCESS) { //FIXME: matching issue
                struct card *c4 = card__init(CARD_ROAD_STRAIGHT);
                c4->pos.x = -1;
                c4->pos.y = 1;
                c4->orientation = NORTH_TO_NORTH;
                if (board__add_card(b, c4) == SUCCESS) {
                    struct card *c5 = card__init(CARD_JUNCTION_FOUR);
                    c5->pos.x = -1;
                    c5->pos.y = 0;
                    c5->orientation = NORTH_TO_NORTH;
                    if (board__add_card(b, c5) == SUCCESS) {
                        struct card *c6 = card__init(CARD_PLAIN_CITY_ROAD);
                        c6->pos.x = 1;
                        c6->pos.y = 2;
                        c6->orientation = NORTH_TO_NORTH;
                        if (board__add_card(b, c6) == SUCCESS) {
                            struct card *c7 = card__init(CARD_MONASTERY_ROAD);
                            c7->pos.x = 1;
                            c7->pos.y = 1;
                            c7->orientation = NORTH_TO_SOUTH;
                            if (board__add_card(b, c7) == SUCCESS) {
                                struct card *c8 = card__init(CARD_JUNCTION_THREE);
                                c8->pos.x = 1;
                                c8->pos.y = 0;
                                c8->orientation = NORTH_TO_NORTH;
                                if (board__add_card(b, c8) == SUCCESS
                                        && card__get_neighbour_number(c1) == 4
                                        && c1->neighbors[NORTH] == set__retrieve(b->cards_set, c2)
                                        && c1->neighbors[WEST] == set__retrieve(b->cards_set, c4)
                                        && c1->neighbors[SOUTH] == b->first_card
                                        && c1->neighbors[EAST] == set__retrieve(b->cards_set, c7)) {
                                    res = TEST_SUCCESS;
                                }
                                card__free(c8);
                            }
                            card__free(c7);
                        }
                        card__free(c6);
                    }
                    card__free(c5);
                }
                card__free(c4);
            }
            card__free(c3);
        }
        card__free(c2);
    }
    card__free(c1);
    board__free(b);
    stack__free(s);

    return res;
}

int main()
{
    printf("----------- TEST BOARD -----------\n");

    print_success(test_board__empty());
    print_success(test_board__add_card());

    return TEST_SUCCESS;
}
