#include <stdlib.h>
#include <stdio.h>
#include "test_utils.h"
#include "../src/common/stack.h"

int test_stack__empty()
{
    printf("%s... ", __func__);

    struct stack *p = stack__empty();

    if (p == NULL) {
        stack__free(p);
        return !SUCCESS;
    }

    stack__free(p);

    return SUCCESS;
}


int test_stack__is_empty_on_empty_stack()
{
    printf("%s... ", __func__);

    struct stack *p = stack__empty();

    if (!stack__is_empty(p)) {
        stack__free(p);
        return !SUCCESS;
    }

    stack__free(p);

    return SUCCESS;
}


int test_stack__is_empty_on_non_empty_stack()
{
    printf("%s... ", __func__);

    struct stack *p = stack__empty();
    enum card_id card = CARD_MONASTERY_ALONE;
    stack__push(p, card);

    if (stack__is_empty(p)) {
        stack__free(p);
        return !SUCCESS;
    }

    stack__free(p);

    return SUCCESS;
}


int test_stack__push_on_non_empty_stack()
{
    printf("%s... ", __func__);

    struct stack *p = stack__empty();
    enum card_id card1 = CARD_MONASTERY_ALONE;
    enum card_id card2 = CARD_CITY_ALL_SIDES;
    enum card_id card3 = CARD_ROAD_STRAIGHT_CITY;
    enum card_id card4 = CARD_CITY_ONE_SIDE;
    stack__push(p, card1);
    stack__push(p, card2);
    stack__push(p, card3);
    stack__push(p, card4);
    stack__push(p, card1);

    if (stack__length(p) != 5) {
        stack__free(p);
        return !SUCCESS;
    }
    stack__free(p);
    return SUCCESS;
}


int test_stack_NULL()
{
    printf("%s... ", __func__);

    struct stack *p = stack__empty();
    enum card_id card = (enum card_id) 999;
    stack__push(p, card);

    if (!stack__is_empty(p)) {
        stack__free(p);
        return !SUCCESS;
    }

    stack__free(p);

    return SUCCESS;
}


int test_stack__push_on_multiple_elements()
{
    printf("%s... ", __func__);

    struct stack *s = stack__empty();
    enum card_id card = CARD_MONASTERY_ALONE;

    for (unsigned int i = 0; i < 500; i++) {
        stack__push(s, card);
    }

    if (stack__length(s) < 500) {
        stack__free(s);
        return !SUCCESS;
    }

    stack__free(s);

    return SUCCESS;
}


int test_stack__peek()
{
    printf("%s... ", __func__);

    struct stack *s = stack__empty();

    if (!stack__is_empty(s) || stack__peek(s) != LAST_CARD) {
        stack__free(s);
        return !SUCCESS;
    }

    stack__push(s, CARD_CITY_THREE);

    if (stack__is_empty(s) || stack__peek(s) != CARD_CITY_THREE) {
        stack__free(s);
        return !SUCCESS;
    }

    stack__free(s);

    return SUCCESS;
}


int test_stack__pop_on_empty_stack()
{
    printf("%s... ", __func__);

    struct stack *s = stack__empty();
    enum card_id unstacked = stack__pop(s);
    enum card_id last_card = LAST_CARD;

    if (last_card != unstacked) {
        stack__free(s);
        return !SUCCESS;
    }

    stack__free(s);

    return SUCCESS;
}


int test_stack__pop_on_non_empty_stack()
{
    printf("%s... ", __func__);

    struct stack *s = stack__empty();
    enum card_id card1 = CARD_MONASTERY_ROAD;
    enum card_id card2 = CARD_MONASTERY_ALONE;
    stack__push(s, card1);
    stack__push(s, card2);
    enum card_id unstacked2 = stack__pop(s);
    enum card_id unstacked1 = stack__pop(s);


    if (!stack__is_empty(s)
            || (unstacked1 != CARD_MONASTERY_ROAD)
            || (unstacked2 != CARD_MONASTERY_ALONE)) {
        stack__free(s);
        return !SUCCESS;
    }

    stack__free(s);

    return SUCCESS;
}


int test_stack_length()
{
    printf("%s... ", __func__);

    struct stack *s = stack__empty();
    enum card_id card1 = CARD_ROAD_TURN_RIGHT_CITY;
    enum card_id card2 = CARD_ROAD_TURN_LEFT_CITY;
    stack__push(s, card1);

    if (stack__length(s) != 1) {
        stack__free(s);
        return !SUCCESS;
    }

    stack__push(s, card2);

    if (stack__length(s) != 2) {
        stack__free(s);
        return !SUCCESS;
    }

    stack__free(s);

    return SUCCESS;
}


int test_stack_debug()
{
    printf("%s (expected 5 6 7 8 5)... ", __func__);

    struct stack *s = stack__empty();
    enum card_id card1 = CARD_CITY_TUNNEL_SHLD;
    enum card_id card2 = CARD_CITY_TUNNEL;
    enum card_id card3 = CARD_PLAIN_TUNNEL;
    enum card_id card4 = CARD_PLAIN_TWO_CITIES;

    stack__push(s, card1);
    stack__push(s, card2);
    stack__push(s, card3);
    stack__push(s, card4);
    stack__push(s, card1);

    stack__debug(s);

    stack__free(s);

    return SUCCESS;
}


int main()
{
    printf("----------- TEST STACK -----------\n");

    print_success(test_stack__empty());
    print_success(test_stack__is_empty_on_empty_stack());
    print_success(test_stack__is_empty_on_non_empty_stack());
    print_success(test_stack__push_on_non_empty_stack());
    print_success(test_stack_NULL());
    print_success(test_stack__push_on_multiple_elements());
    print_success(test_stack__peek());
    print_success(test_stack__pop_on_empty_stack());
    print_success(test_stack__pop_on_non_empty_stack());
    print_success(test_stack_length());
    print_success(test_stack_debug());

    return SUCCESS;
}
