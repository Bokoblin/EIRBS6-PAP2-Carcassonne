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
  
    for (enum place p = POS_NORTH_EAST; p <= POS_EAST_NORTH ; p++) {
        if (card__get_relative_area(c, p) != FIELD) {
            card__free(c);
            return !TEST_SUCCESS;
        }
    }

    if (card__get_relative_area(c, POS_CENTER) != ABBEY) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    if (card__get_relative_area(c, (enum place) 50) != INVALID_AREA) {
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

    if (card__get_position_at_direction(c, NORTH).x != 5
            || card__get_position_at_direction(c, NORTH).y != -8) {
        card__free(c);
        return !TEST_SUCCESS;
    }
    if (card__get_position_at_direction(c, SOUTH).x != 5
            || card__get_position_at_direction(c, SOUTH).y != -10) {
        card__free(c);
        return !TEST_SUCCESS;
    }
    if (card__get_position_at_direction(c, WEST).x != 4
            || card__get_position_at_direction(c, WEST).y != -9) {
        card__free(c);
        return !TEST_SUCCESS;
    }
    if (card__get_position_at_direction(c, EAST).x != 6
            || card__get_position_at_direction(c, EAST).y != -9) {
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

    if (!card__are_matching_directions(c1, c2, WEST, EAST)) {
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

int test_card__link_at_side()
{
    printf("%s... ", __func__);

    struct card *c1 = card__init(CARD_MONASTERY_ROAD);
    struct card *c2 = card__init(CARD_JUNCTION_THREE);

    if (card__link_at_directions(c1, c2, SOUTH, EAST)
            && (c1->neighbors[SOUTH] != c2
            ||c2->neighbors[EAST] != c1)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__link_at_directions(c1, c2, SOUTH, EAST)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__link_at_directions(c1, c1, EAST, WEST)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__link_at_directions(c1, c2, EAST + 1, EAST)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    card__free(c1);
    card__free(c2);

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

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
