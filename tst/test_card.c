#include <stdlib.h>
#include "test_utils.h"
#include "../src/common/common_interface.h"
#include "../src/common/card.h"
#include "../src/common/ADT/stack.h"
#include "../src/common/utils.h"

////////////////////////////////////////////////////////////////////
///     OPERATORS FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

void* operator_copy(void* given_card)
{
    enum card_id *_given_card = given_card;
    enum card_id *new_card = safe_malloc(sizeof(enum card_id));
    *new_card = *_given_card;
    return new_card;
}

void operator_delete(void* given_card)
{
    free(given_card);
}

void operator_debug(void* given_card)
{
    enum card_id *_given_card = given_card;
    printf("%d ", *_given_card);
}


////////////////////////////////////////////////////////////////////
///     CARDS FUNCTIONS TESTS
////////////////////////////////////////////////////////////////////

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


int test_card__place()
{
    printf("%s... ", __func__);

    //TODO : test_card__place

    printf("- Not tested yet - ");

    return !TEST_SUCCESS;
}


int test_card__get_area()
{
    printf("%s... ", __func__);

    struct card *c = card__init(CARD_MONASTERY_ALONE);

    for (int i = 0 ; i < (MAX_ZONES-1) ; i++) {
        if (card__get_area(c, (enum place) i) != FIELD) {
            card__free(c);
            return !TEST_SUCCESS;
        }
    }

    if (card__get_area(c, POS_CENTER) != ABBEY) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    card__free(c);

    return TEST_SUCCESS;

}


int test_card__are_matching_direction_success_case()
{
    printf("%s... ", __func__);

    struct card *c1 = card__init(CARD_MONASTERY_ALONE);
    struct card *c2 = card__init(CARD_MONASTERY_ROAD);

    if (card__are_matching_direction(c1, c2, EAST)) {
        card__free(c1);
        card__free(c2);
        return TEST_SUCCESS;
    }

    card__free(c1);
    card__free(c2);

    return !TEST_SUCCESS;
}

int test_card__are_matching_direction_failure_case()
{
    printf("%s... ", __func__);

    struct card *c1 = card__init(CARD_MONASTERY_ALONE);
    struct card *c2 = card__init(CARD_PLAIN_CITY_ROAD);

    if (!card__are_matching_direction(c1, c2, EAST)) {
        card__free(c1);
        card__free(c2);
        return TEST_SUCCESS;
    }

    card__free(c1);
    card__free(c2);

    return !TEST_SUCCESS;
}

int test_card__link_at_direction()
{
    printf("%s... ", __func__);

    struct card *c1 = card__init(CARD_MONASTERY_ALONE);
    struct card *c2 = card__init(CARD_MONASTERY_ROAD);

    card__link_at_direction(c1, c2, EAST);

    if ((c1->neighbors[EAST] == c2) && (c2->neighbors[WEST] == c1)) {
        card__free(c1);
        card__free(c2);
        return TEST_SUCCESS;
    }

    card__free(c1);
    card__free(c2);

    return !TEST_SUCCESS;
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
    card__set_orientation(c, NORTH_TO_WEST);

    if (c->orientation == NORTH_TO_WEST) {
        card__free(c);
        return TEST_SUCCESS;
    }

    card__free(c);

    return !TEST_SUCCESS;
}



int main()
{
    printf("----------- TEST CARD -----------\n");

    print_success(test_card__init());
    print_success(test_card__place());
    print_success(test_card__get_area());
    print_success(test_card__are_matching_direction_success_case());
    print_success(test_card__are_matching_direction_failure_case());
    print_success(test_card__link_at_direction());
    print_success(test_card__draw());
    print_success(test_card__set_orientation());

    return TEST_SUCCESS;
}
