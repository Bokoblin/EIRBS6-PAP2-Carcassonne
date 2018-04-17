#include <stdlib.h>
#include "test_utils.h"
#include "../src/server/board.h"
#include "../src/common/ADT/set.h"
#include "../src/common/utils.h"
#include "../src/server/function_pointers.h"

////////////////////////////////////////////////////////////////////
///     OPERATORS FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

void cards_set_debug_op(struct card* c)
{
    printf("\n=== Display card ===\n");
    printf("\ttype id: %d\n", c->type.id);
    printf("\tareas: { ");
    for (int i = 0; i < MAX_ZONES; i++) printf("%d, ", c->type.areas[i]);
    printf("}\n");
    printf("\tposition: { %d, %d }\n", c->pos.x, c->pos.y);
    printf("\tneighbours: { ");
    for (int i = 0; i < DIRECTION_NUMBER; i++)
        if (c->neighbors[i] != NULL)
            printf("%d, ", c->neighbors[i]->type.id);
    printf("}\n");
    printf("\torientation: %d\n", c->orientation);
}

////////////////////////////////////////////////////////////////////
///     BOARD FUNCTIONS TESTS
////////////////////////////////////////////////////////////////////

int test_board__empty()
{
    printf("%s... ", __func__);

    struct board* b1 = board__init(NULL);

    if (b1->first_card != NULL || !set__is_empty(b1->cards_set) || !set__is_empty(b1->meeples_set)) {
        board__free(b1);
        return !TEST_SUCCESS;
    }

    struct stack* s = stack__empty(cards_set_copy_op, cards_set_delete_op, cards_set_debug_op);
    struct card* starting_card = card__init(CARD_ROAD_STRAIGHT_CITY);
    stack__push(s, starting_card);

    board__free(b1);
    b1 = board__init(s);

    if (b1->first_card == NULL || set__is_empty(b1->cards_set) || !set__is_empty(b1->meeples_set)) {
        board__free(b1);
        stack__free(s);
        card__free(starting_card);
        return !TEST_SUCCESS;
    }

    board__free(b1);
    stack__free(s);
    card__free(starting_card);

    return TEST_SUCCESS;
}

int test_board__add_card()
{
    printf("%s... ", __func__);

    struct board *b = board__init(NULL);
    struct card *c = card__init(CARD_PLAIN_CITY);
    c->pos.x = 0;
    c->pos.y = -1;

    //First test: adding in empty set

    if (board__add_card(b, c) == SUCCESS || card__get_neighbour_number(c) != 0) {
        board__free(b);
        card__free(c);
        return !TEST_SUCCESS;
    }

    board__free(b);

    //Second test: adding in non empty set (1) with no match

    struct stack* s = stack__empty(cards_set_copy_op, cards_set_delete_op, cards_set_debug_op);
    struct card* starting_card = card__init(CARD_ROAD_STRAIGHT_CITY);
    stack__push(s, starting_card);

    b = board__init(s);
    c->type = card__id_to_type(CARD_CITY_ALL_SIDES);

    if (board__add_card(b, c) == SUCCESS || card__get_neighbour_number(c) != 0) {
        board__free(b);
        stack__free(s);
        card__free(c);
        card__free(starting_card);
        return !TEST_SUCCESS;
    }

    //Third test: adding in non empty set with a match

    c->type = card__id_to_type(CARD_ROAD_TURN);

    if (board__add_card(b, c) != SUCCESS || card__get_neighbour_number(c) != 1) {
        board__free(b);
        stack__free(s);
        card__free(c);
        card__free(starting_card);
        return !TEST_SUCCESS;
    }

    //Fourth test: adding in center of 4 tiles for full match
    //TODO: fourth test

    board__free(b);
    stack__free(s);
    card__free(c);
    card__free(starting_card);

    return TEST_SUCCESS;
}

int main()
{
    printf("----------- TEST BOARD -----------\n");

    print_success(test_board__empty());
    print_success(test_board__add_card());

    return TEST_SUCCESS;
}
