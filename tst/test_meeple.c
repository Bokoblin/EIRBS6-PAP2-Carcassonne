#include "common_tests_utils.h"
#include "../src/common/meeple.h"

int test_meeple__init_not_null()
{
    printf("%s... ", __func__);

    struct card *c = card__init(CARD_MONASTERY_ROAD);
    struct meeple* m = meeple__init(0, c, POS_CENTER);

    if (m == NULL || c == NULL) {
        if (m == NULL) card__free(c);
        if (c == NULL) meeple__free(m);
        return  !TEST_SUCCESS;
    }

    card__free(c);
    meeple__free(m);

    return TEST_SUCCESS;
}

int test_meeple__init_valid_card_default_orientation()
{
    printf("%s... ", __func__);

    int test_res = TEST_SUCCESS;

    struct card *c = card__init(CARD_MONASTERY_ROAD);
    c->orientation = NORTH_IS_NORTH_SIDE;

    enum place p1 = POS_CENTER;
    struct meeple* m = meeple__init(0, c, p1);
    if (m->player_id != 0) test_res = !TEST_SUCCESS;
    if (m->related_card != c) test_res = !TEST_SUCCESS;
    if (m->position != p1) test_res = !TEST_SUCCESS;
    if (m->belonging_area != ABBEY) test_res = !TEST_SUCCESS;
    meeple__free(m);

    enum place p2 = POS_NORTH;
    struct meeple* m2 = meeple__init(1, c, p2);
    if (m2->player_id != 1) test_res = !TEST_SUCCESS;
    if (m2->related_card != c) test_res = !TEST_SUCCESS;
    if (m2->position != p2) test_res = !TEST_SUCCESS;
    if (m2->belonging_area != FIELD) test_res = !TEST_SUCCESS;
    meeple__free(m2);

    enum place p3 = POS_SOUTH;
    struct meeple* m3 = meeple__init(2, c, p3);
    if (m3->player_id != 2) test_res = !TEST_SUCCESS;
    if (m3->related_card != c) test_res = !TEST_SUCCESS;
    if (m3->position != p3) test_res = !TEST_SUCCESS;
    if (m3->belonging_area != ROAD) test_res = !TEST_SUCCESS;
    meeple__free(m3);

    card__free(c);

    return test_res;
}

int test_meeple__init_valid_card_other_orientation()
{
    printf("%s... ", __func__);

    int test_res = TEST_SUCCESS;

    struct card *c = card__init(CARD_ROAD_TURN_RIGHT_CITY);
    c->orientation = NORTH_IS_WEST_SIDE;

    enum place p1 = POS_CENTER;
    struct meeple* m = meeple__init(0, c, p1);
    if (m->player_id != 0) test_res = !TEST_SUCCESS;
    if (m->related_card != c) test_res = !TEST_SUCCESS;
    if (m->position != p1) test_res = !TEST_SUCCESS;
    if (m->belonging_area != FIELD) test_res = !TEST_SUCCESS;
    meeple__free(m);

    enum place p2 = POS_NORTH;
    struct meeple* m2 = meeple__init(1, c, p2);
    if (m2->player_id != 1) test_res = !TEST_SUCCESS;
    if (m2->related_card != c) test_res = !TEST_SUCCESS;
    if (m2->position != p2) test_res = !TEST_SUCCESS;
    if (m2->belonging_area != FIELD) test_res = !TEST_SUCCESS;
    meeple__free(m2);

    enum place p3 = POS_SOUTH;
    struct meeple* m3 = meeple__init(2, c, p3);
    if (m3->player_id != 2) test_res = !TEST_SUCCESS;
    if (m3->related_card != c) test_res = !TEST_SUCCESS;
    if (m3->position != p3) test_res = !TEST_SUCCESS;
    if (m3->belonging_area != ROAD) test_res = !TEST_SUCCESS;
    meeple__free(m3);

    enum place p4 = POS_EAST_NORTH;
    struct meeple* m4 = meeple__init(3, c, p4);
    if (m4->player_id != 3) test_res = !TEST_SUCCESS;
    if (m4->related_card != c) test_res = !TEST_SUCCESS;
    if (m4->position != p4) test_res = !TEST_SUCCESS;
    if (m4->belonging_area != CITY) test_res = !TEST_SUCCESS;
    meeple__free(m4);

    enum place p5 = POS_SOUTH_WEST;
    struct meeple* m5 = meeple__init(4, c, p5);
    if (m5->player_id != 4) test_res = !TEST_SUCCESS;
    if (m5->related_card != c) test_res = !TEST_SUCCESS;
    if (m5->position != p5) test_res = !TEST_SUCCESS;
    if (m5->belonging_area != FIELD) test_res = !TEST_SUCCESS;
    meeple__free(m5);

    card__free(c);

    return test_res;
}

int test_meeple__init_valid_card_invalid_place()
{
    printf("%s... ", __func__);

    int test_res = TEST_SUCCESS;

    struct card *c = card__init(CARD_ROAD_TURN_RIGHT_CITY);

    enum place p1 = (enum place) 49;
    struct meeple* m = meeple__init(0, c, p1);
    if (m->player_id != 0) test_res = !TEST_SUCCESS;
    if (m->related_card != c) test_res = !TEST_SUCCESS;
    if (m->position != p1) test_res = !TEST_SUCCESS;
    if (m->belonging_area != INVALID_AREA) test_res = !TEST_SUCCESS;
    meeple__free(m);

    card__free(c);

    return test_res;
}

int test_meeple__init_fail_no_needed()
{
    printf("%s... ", __func__);

    int test_res = TEST_SUCCESS;

    struct card *c = card__init(CARD_ROAD_TURN_RIGHT_CITY);

    enum place p1 = NO_MEEPLE;
    struct meeple* m = meeple__init(0, c, p1);

    if (m != NULL) {
        meeple__free(m);
        test_res = !TEST_SUCCESS;
    }

    card__free(c);

    return test_res;
}

int main()
{
    printf("----------- TEST MEEPLE -----------\n");

    int nb_success = 0;
    int nb_tests = 0;

    print_test_result(test_meeple__init_not_null(), &nb_success, &nb_tests);
    print_test_result(test_meeple__init_valid_card_default_orientation(), &nb_success, &nb_tests);
    print_test_result(test_meeple__init_valid_card_other_orientation(), &nb_success, &nb_tests);
    print_test_result(test_meeple__init_valid_card_invalid_place(), &nb_success, &nb_tests);
    print_test_result(test_meeple__init_fail_no_needed(), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
