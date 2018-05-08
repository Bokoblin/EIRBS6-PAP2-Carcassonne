#include "common_tests_utils.h"
#include "../src/common/card.h"
#include "../src/common/ADT/stack.h"
#include "../src/common/utils.h"

int test_card__init()
{
    printf("%s... ", __func__);

    struct card* c = card__init(CARD_JUNCTION_CITY);
    if (c->type.id != CARD_JUNCTION_CITY) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    if (c->direction != DEFAULT_DIRECTION) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    card__free(c);

    return TEST_SUCCESS;
}

int test_card__get_area()
{
    printf("%s... ", __func__);

    struct card *c = card__init(CARD_MONASTERY_ALONE);
  
    for (int i = 0 ; i <= POS_NORTH_EAST ; i++) {
        if (card__get_area(c, (enum place) i) != FIELD) {
            card__free(c);
            return !TEST_SUCCESS;
        }
    }

    if (card__get_area(c, POS_CENTER) != ABBEY) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    if (card__get_area(c, (enum place) 50) != INVALID_AREA) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    card__free(c);

    return TEST_SUCCESS;

}

int test_card__get_neighbour_number()
{
    printf("%s... ", __func__);

    struct card *c = card__init(CARD_MONASTERY_ALONE);

    if (card__get_neighbour_number(c) != 0) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    c->neighbors[0] = c; //stupid but enough for the test

    if (card__get_neighbour_number(c) != 1) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    card__free(c);

    return TEST_SUCCESS;
}

int test_card__get_position_at_side()
{
    printf("%s... ", __func__);

    struct card *c = card__init(CARD_MONASTERY_ALONE);
    c->pos = (struct position){ 5, -9 };

    if (card__get_position_at_side(c, NORTH_SIDE).x != 5
            || card__get_position_at_side(c, NORTH_SIDE).y != -8) {
        card__free(c);
        return !TEST_SUCCESS;
    }
    if (card__get_position_at_side(c, SOUTH_SIDE).x != 5
            || card__get_position_at_side(c, SOUTH_SIDE).y != -10) {
        card__free(c);
        return !TEST_SUCCESS;
    }
    if (card__get_position_at_side(c, WEST_SIDE).x != 4
            || card__get_position_at_side(c, WEST_SIDE).y != -9) {
        card__free(c);
        return !TEST_SUCCESS;
    }
    if (card__get_position_at_side(c, EAST_SIDE).x != 6
            || card__get_position_at_side(c, EAST_SIDE).y != -9) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    card__free(c);
    return TEST_SUCCESS;
}

int test_card__are_matching_sides_success()
{
    printf("%s... ", __func__);

    struct card *c1 = card__init(CARD_CITY_THREE_SHLD);
    struct card *c2 = card__init(CARD_CITY_ALL_SIDES);

    if (!card__are_matching_sides(c1, c2, WEST_SIDE, EAST_SIDE)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    card__free(c1);
    card__free(c2);

    return TEST_SUCCESS;
}

int test_card__are_matching_sides_failure()
{
    printf("%s... ", __func__);

    struct card *c1 = card__init(CARD_MONASTERY_ALONE);
    struct card *c2 = card__init(CARD_JUNCTION_THREE);

    if (card__are_matching_sides(c1, c1, EAST_SIDE, WEST_SIDE)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__are_matching_sides(c1, c2, EAST_SIDE, SOUTH_SIDE)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    card__free(c1);
    card__free(c2);

    return TEST_SUCCESS;
}

int test_card__link_at_side()
{
    printf("%s... ", __func__);

    struct card *c1 = card__init(CARD_MONASTERY_ROAD);
    struct card *c2 = card__init(CARD_JUNCTION_THREE);

    if (card__link_at_sides(c1, c2, SOUTH_SIDE, EAST_SIDE)
            && (c1->neighbors[SOUTH_SIDE] != c2
            ||c2->neighbors[EAST_SIDE] != c1)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__link_at_sides(c1, c2, SOUTH_SIDE, EAST_SIDE)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__link_at_sides(c1, c1, EAST_SIDE, WEST_SIDE)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__link_at_sides(c1, c2, EAST_SIDE + 1, EAST_SIDE)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    card__free(c1);
    card__free(c2);

    return TEST_SUCCESS;
}


int test_card__draw()
{
    printf("%s... ", __func__);

    struct stack *s = stack__empty(&operator_copy, &operator_delete, &operator_debug);

    enum card_id id =  CARD_JUNCTION_CITY;

    stack__push(s, &id);

    enum card_id cid = card__draw(s);

    if (cid != CARD_JUNCTION_CITY) {
        stack__free(s);
        return !TEST_SUCCESS;
    }

    stack__free(s);

    return TEST_SUCCESS;
}


int main()
{
    printf("----------- TEST CARD -----------\n");

    int nb_success = 0;
    int nb_tests = 0;

    print_test_result(test_card__init(), &nb_success, &nb_tests);
    print_test_result(test_card__get_area(), &nb_success, &nb_tests);
    print_test_result(test_card__get_neighbour_number(), &nb_success, &nb_tests);
    print_test_result(test_card__get_position_at_side(), &nb_success, &nb_tests);
    print_test_result(test_card__are_matching_sides_success(), &nb_success, &nb_tests);
    print_test_result(test_card__are_matching_sides_failure(), &nb_success, &nb_tests);
    print_test_result(test_card__link_at_side(), &nb_success, &nb_tests);
    print_test_result(test_card__draw(), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
