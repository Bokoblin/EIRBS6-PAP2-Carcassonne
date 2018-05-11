#include "common_tests_utils.h"
#include "../src/common/zone.h"

int test_zone__empty()
{
    printf("%s... ", __func__);
    int result = TEST_SUCCESS;

    struct zone* z = zone__empty(ABBEY);

    if (z->area != ABBEY) result = !TEST_SUCCESS;
    if (z->score != 0) result = !TEST_SUCCESS;
    if (z->holes != 0) result = !TEST_SUCCESS;
    if (!set__is_empty(z->area_set)) result = !TEST_SUCCESS;
    if (!set__is_empty(z->meeple_set)) result = !TEST_SUCCESS;

    zone__free(z);

    return result;
}

int test_zone__cards_available_zones()
{
    printf("%s... NOT IMPLEMENTED YET - ", __func__);

    return !TEST_SUCCESS;
}

int test_zone__matching_area_set()
{
    printf("%s... NOT IMPLEMENTED YET - ", __func__);

    return !TEST_SUCCESS;
}

int test_zone__add_areas()
{
    printf("%s... NOT IMPLEMENTED YET - ", __func__);

    return !TEST_SUCCESS;
}

int test_zone__is_fusion_required()
{
    printf("%s... NOT IMPLEMENTED YET - ", __func__);

    return !TEST_SUCCESS;
}

int test_zone__fusion_zone()
{
    printf("%s... NOT IMPLEMENTED YET - ", __func__);

    return !TEST_SUCCESS;
}

int test_zone__update()
{
    printf("%s... NOT IMPLEMENTED YET - ", __func__);

    return !TEST_SUCCESS;
}

int test_zone__add_meeple()
{
    printf("%s... NOT IMPLEMENTED YET - ", __func__);

    return !TEST_SUCCESS;
}

int main()
{
    printf("----------- TEST ZONE -----------\n");

    int nb_success = 0;
    int nb_tests = 0;

    print_test_result(test_zone__empty(), &nb_success, &nb_tests);
    print_test_result(test_zone__cards_available_zones(), &nb_success, &nb_tests);
    print_test_result(test_zone__matching_area_set(), &nb_success, &nb_tests);
    print_test_result(test_zone__add_areas(), &nb_success, &nb_tests);
    print_test_result(test_zone__is_fusion_required(), &nb_success, &nb_tests);
    print_test_result(test_zone__fusion_zone(), &nb_success, &nb_tests);
    print_test_result(test_zone__update(), &nb_success, &nb_tests);
    print_test_result(test_zone__add_meeple(), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
