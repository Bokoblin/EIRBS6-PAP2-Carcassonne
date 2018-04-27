#include "common_tests_utils.h"
#include "../src/common/deck.h"

int test_init_deck()
{
    printf("%s... ", __func__);

    struct stack *d = stack__empty(&operator_copy, &operator_delete, &operator_debug);

    if (d == NULL)
        return !TEST_SUCCESS;

    init_deck(d);

    if (stack__length(d) != CARDS_NUMBER) {
        stack__free(d);
        return !TEST_SUCCESS;
    }

    int card_occurrence[24];
    for (unsigned int i = 0; i < 24; i++)
        card_occurrence[i] = 0;

    while (!stack__is_empty(d)) {
        enum card_id *ci = stack__pop(d);
        card_occurrence[*ci]++;
        free(ci);
    }

    if (card_occurrence[CARD_ROAD_TURN] != 9
            || card_occurrence[CARD_ROAD_TURN_RIGHT_CITY] != 3
            || card_occurrence[CARD_PLAIN_CITY_ROAD_SHLD] != 2
            || card_occurrence[CARD_CITY_THREE_ROAD] != 1
            || card_occurrence[CARD_CITY_THREE_SHLD] != 1
            || card_occurrence[CARD_PLAIN_CITY_ROAD] != 3
            || card_occurrence[CARD_JUNCTION_CITY] != 3
            || card_occurrence[CARD_ROAD_STRAIGHT] != 8
            || card_occurrence[CARD_JUNCTION_THREE] != 4
            || card_occurrence[CARD_CITY_ONE_SIDE] != 5
            || card_occurrence[CARD_PLAIN_TWO_CITIES] != 2
            || card_occurrence[CARD_CITY_THREE] != 3
            || card_occurrence[CARD_MONASTERY_ALONE] != 4
            || card_occurrence[CARD_MONASTERY_ROAD] != 2
            || card_occurrence[CARD_PLAIN_CITY] != 3
            || card_occurrence[CARD_CITY_TUNNEL_SHLD] != 2
            || card_occurrence[CARD_ROAD_STRAIGHT_CITY] != 4
            || card_occurrence[CARD_ROAD_TURN_LEFT_CITY] != 3
            || card_occurrence[CARD_CITY_TUNNEL] != 1
            || card_occurrence[CARD_CITY_THREE_ROAD_SHLD] != 2
            || card_occurrence[CARD_CITY_ALL_SIDES] != 1
            || card_occurrence[CARD_JUNCTION_FOUR] != 1
            || card_occurrence[CARD_PLAIN_CITY_SHLD] != 2
            || card_occurrence[CARD_PLAIN_TUNNEL] != 3
            ) {
        stack__free(d);
        return !TEST_SUCCESS;
    }

    stack__free(d);

    return TEST_SUCCESS;
}

int main()
{
    printf("----------- TEST DECK -----------\n");

    int nb_success = 0;
    int nb_tests = 0;

    print_test_result(test_init_deck(), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
