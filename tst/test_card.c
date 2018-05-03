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

    if (c->orientation != DEFAULT_ORIENTATION) {
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


int test_card__are_matching_direction_success()
{
    printf("%s... ", __func__);

    struct card *c1 = card__init(CARD_CITY_THREE_SHLD);
    struct card *c2 = card__init(CARD_CITY_ALL_SIDES);

    if (!card__are_matching_directions(c1, c2, WEST, EAST)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    card__free(c1);
    card__free(c2);

    return TEST_SUCCESS;
}

int test_card__are_matching_direction_failure_case()
{
    printf("%s... ", __func__);

    struct card *c1 = card__init(CARD_MONASTERY_ALONE);
    struct card *c2 = card__init(CARD_JUNCTION_THREE);

    if (card__are_matching_directions(c1, c1, EAST, WEST)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__are_matching_directions(c1, c2, EAST, SOUTH)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    card__free(c1);
    card__free(c2);

    return TEST_SUCCESS;
}

int test_card__link_at_direction()
{
    printf("%s... ", __func__);

    struct card *c1 = card__init(CARD_MONASTERY_ALONE);
    struct card *c2 = card__init(CARD_JUNCTION_THREE);

    if (card__are_matching_directions(c1, c2, NORTH, EAST) != SUCCESS) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__link_at_directions(c1, c1, EAST, WEST) == SUCCESS) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__link_at_directions(c1, c2, EAST, SOUTH)) {
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


int test_card__set_orientation()
{
    printf("%s... ", __func__);

    struct card *c = card__init(CARD_JUNCTION_CITY);
    card__set_orientation(c, NORTH_IS_WEST_SIDE);

    if (c->orientation == NORTH_IS_WEST_SIDE) {
        card__free(c);
        return TEST_SUCCESS;
    }

    card__free(c);

    return !TEST_SUCCESS;
}



int main()
{
    printf("----------- TEST CARD -----------\n");

    int nb_success = 0;
    int nb_tests = 0;

    print_test_result(test_card__init(), &nb_success, &nb_tests);
    print_test_result(test_card__get_area(), &nb_success, &nb_tests);
    print_test_result(test_card__get_neighbour_number(), &nb_success, &nb_tests);
    print_test_result(test_card__are_matching_direction_success(), &nb_success, &nb_tests);
    print_test_result(test_card__are_matching_direction_failure_case(), &nb_success, &nb_tests);
    print_test_result(test_card__link_at_direction(), &nb_success, &nb_tests);
    print_test_result(test_card__draw(), &nb_success, &nb_tests);
    print_test_result(test_card__set_orientation(), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
