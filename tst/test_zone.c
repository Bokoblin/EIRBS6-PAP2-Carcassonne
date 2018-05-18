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
    printf("%s... ", __func__);
    int result = TEST_SUCCESS;

    struct card *c = card__init(CARD_MONASTERY_ROAD);

    struct set *p_place = zone__cards_available_zones(c);

    //if (!set__find(p_place, (void *) &allowed_positions[c->type.id][0])) result = !TEST_SUCCESS; //FIXME?
    //if (!set__find(p_place, (void *) &allowed_positions[c->type.id][1])) result = !TEST_SUCCESS; //FIXME?
    //if (!set__find(p_place, (void *) &allowed_positions[c->type.id][2])) result = !TEST_SUCCESS; //FIXME?
    if (set__size(p_place) != 3) result = !TEST_SUCCESS;

    set__free(p_place);
    card__free(c);

    return result;
}

int test_zone__matching_area_set()
{
    printf("%s... ", __func__);
    int result = TEST_SUCCESS;

    enum area_type at = FIELD;
    struct set *area_set = zone__matching_area_set(&at);

    //if (!set__find(area_set, &at)) result = !TEST_SUCCESS; //FIXME?
    if (set__size(area_set) != 1) result = !TEST_SUCCESS;

    set__free(area_set);

    return result;
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
    printf("%s... ", __func__);

    struct zone *z = zone__empty(FIELD);

    struct card *c = card__init(CARD_CITY_ONE_SIDE);

    struct meeple *m = meeple__init(1, c, POS_CENTER);

    if (!zone__add_meeple(z, m)){
        card__free(c);
        zone__free(z);
        return TEST_SUCCESS;
    }

    card__free(c);
    zone__free(z);
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
    //print_test_result(test_zone__add_areas(), &nb_success, &nb_tests);            //TODO
    //print_test_result(test_zone__is_fusion_required(), &nb_success, &nb_tests);   //TODO
    //print_test_result(test_zone__fusion_zone(), &nb_success, &nb_tests);          //TODO
    //print_test_result(test_zone__update(), &nb_success, &nb_tests);               //TODO
    print_test_result(test_zone__add_meeple(), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
