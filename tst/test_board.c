#include "common_tests_utils.h"
#include "../src/server/board.h"
#include "../src/common/ADT/set.h"

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
    b1->first_card->orientation = NORTH_IS_EAST_SIDE;

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

    //=== First test: adding in empty set

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

    //=== Second test: adding in non empty set (1) with no match

    struct stack* s = stack__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id ci_first = CARD_ROAD_STRAIGHT_CITY;
    stack__push(s, &ci_first);
    b = board__init(s);
    b->first_card->orientation = NORTH_IS_EAST_SIDE;

    if (board__add_card(b, c1) == SUCCESS || card__get_neighbour_number(c1) != 0) {
        board__free(b);
        stack__free(s);
        card__free(c1);
        return !TEST_SUCCESS;
    }

    //=== Third test: adding in non empty set with a match

    c1->type = card__id_to_type(CARD_CITY_TUNNEL);

    if (board__add_card(b, c1) != SUCCESS || card__get_neighbour_number(c1) != 1
            || c1->neighbors[SOUTH] != b->first_card) {
        board__free(b);
        stack__free(s);
        card__free(c1);
        return !TEST_SUCCESS;
    }

    //=== Fourth test adding same twice

    if (board__add_card(b, c1) == SUCCESS) {
        board__free(b);
        stack__free(s);
        card__free(c1);
        return !TEST_SUCCESS;
    }

    //Fifth test: adding until having a full surrounding for CARD_CITY_TUNNEL

    int test_result = EXIT_SUCCESS;
    struct card *c2 = NULL, *c3 = NULL, *c4 = NULL, *c5 = NULL, *c6 = NULL, *c7 = NULL, *c8 = NULL;

    c2 = card__init(CARD_CITY_THREE);
    c2->pos.x = 0;
    c2->pos.y = 2;
    c2->orientation = NORTH_IS_NORTH_SIDE;
    if (board__add_card(b, c2) != SUCCESS) { //must fail
        c2->orientation = NORTH_IS_SOUTH_SIDE;
        if (board__add_card(b, c2) == SUCCESS) { //must pass like beyond ones
            c3 = card__init(CARD_ROAD_TURN_RIGHT_CITY);
            c3->pos.x = -1;
            c3->pos.y = 2;
            c3->orientation = NORTH_IS_WEST_SIDE;
            if (board__add_card(b, c3) == SUCCESS) {
                c4 = card__init(CARD_ROAD_STRAIGHT);
                c4->pos.x = -1;
                c4->pos.y = 1;
                c4->orientation = NORTH_IS_NORTH_SIDE;
                if (board__add_card(b, c4) == SUCCESS) {
                    c5 = card__init(CARD_JUNCTION_FOUR);
                    c5->pos.x = -1;
                    c5->pos.y = 0;
                    c5->orientation = NORTH_IS_NORTH_SIDE;
                    if (board__add_card(b, c5) == SUCCESS) {
                        c6 = card__init(CARD_PLAIN_CITY_ROAD);
                        c6->pos.x = 1;
                        c6->pos.y = 2;
                        c6->orientation = NORTH_IS_NORTH_SIDE;
                        if (board__add_card(b, c6) == SUCCESS) {
                            c7 = card__init(CARD_MONASTERY_ROAD);
                            c7->pos.x = 1;
                            c7->pos.y = 1;
                            c7->orientation = NORTH_IS_SOUTH_SIDE;
                            if (board__add_card(b, c7) == SUCCESS) {
                                c8 = card__init(CARD_JUNCTION_THREE);
                                c8->pos.x = 1;
                                c8->pos.y = 0;
                                c8->orientation = NORTH_IS_NORTH_SIDE;
                                if (board__add_card(b, c8) != SUCCESS)
                                    test_result = !TEST_SUCCESS;
                            }
                        }
                    }
                }
            }
        }
    }

    //=== Sixth test : Card link verification

    //NOTE: With previous test, we have made a 3*3 grid of cards, coordinates are indicated below
    //We recover below the current cards because the set in board stores copies of the cards we have added :

    struct card * cur_c0 = b->first_card; //SC
    struct card * cur_c1 = set__retrieve(b->cards_set, c1); //C
    struct card * cur_c2 = set__retrieve(b->cards_set, c2); //NC
    struct card * cur_c3 = set__retrieve(b->cards_set, c3); //NW
    struct card * cur_c4 = set__retrieve(b->cards_set, c4); //CW
    struct card * cur_c5 = set__retrieve(b->cards_set, c5); //SW
    struct card * cur_c6 = set__retrieve(b->cards_set, c6); //NE
    struct card * cur_c7 = set__retrieve(b->cards_set, c7); //CE
    struct card * cur_c8 = set__retrieve(b->cards_set, c8); //SE

    //Neighbours count test :

    if (card__get_neighbour_number(cur_c0) != 3) test_result = !TEST_SUCCESS;
    if (card__get_neighbour_number(cur_c1) != 4) test_result = !TEST_SUCCESS;
    if (card__get_neighbour_number(cur_c2) != 3) test_result = !TEST_SUCCESS;
    if (card__get_neighbour_number(cur_c3) != 2) test_result = !TEST_SUCCESS;
    if (card__get_neighbour_number(cur_c4) != 3) test_result = !TEST_SUCCESS;
    if (card__get_neighbour_number(cur_c5) != 2) test_result = !TEST_SUCCESS;
    if (card__get_neighbour_number(cur_c6) != 2) test_result = !TEST_SUCCESS;
    if (card__get_neighbour_number(cur_c7) != 3) test_result = !TEST_SUCCESS;
    if (card__get_neighbour_number(cur_c8) != 2) test_result = !TEST_SUCCESS;

    //Linking test :

    if (cur_c0->neighbors[NORTH] == cur_c1 && cur_c0->neighbors[WEST] == cur_c5
            && cur_c0->neighbors[SOUTH] == NULL && cur_c0->neighbors[EAST] == cur_c8)
        test_result = !TEST_SUCCESS;

    //TODO: 8 others


    //== Freeing resources

    card__free(c1);
    card__free(c2);
    card__free(c3);
    card__free(c4);
    card__free(c5);
    card__free(c6);
    card__free(c7);
    card__free(c8);
    board__free(b);
    stack__free(s);

    return test_result;
}

int main()
{
    printf("----------- TEST BOARD -----------\n");

    print_success(test_board__empty());
    print_success(test_board__add_card());

    return TEST_SUCCESS;
}
