#include <stdlib.h>
#include <stdio.h>
#include "test_utils.h"
#include "../src/common/queue.h"
#include "../src/common/common_interface.h"

////////////////////////////////////////////////////////////////////
///     USER FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

void* operator_copy(void* given_card)
{
    enum card_id *_given_card = given_card;
    enum card_id *new_card = malloc(sizeof(enum card_id));
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
    printf("%d", *_given_card);
}

////////////////////////////////////////////////////////////////////
///     queue FUNCTIONS TESTS
////////////////////////////////////////////////////////////////////


int test_queue__empty()
{
    printf("%s... ", __func__);

    struct queue *p = queue__empty(&operator_copy, &operator_delete, &operator_debug);

    if (p == NULL) {
        queue__free(p);
        return !SUCCESS;
    }

    queue__free(p);

    return SUCCESS;
}

int test_queue__is_empty_on_empty_queue()
{
    printf("%s... ", __func__);

    struct queue *p = queue__empty(&operator_copy, &operator_delete, &operator_debug);

    if (!queue__is_empty(p)) {
        queue__free(p);
        return !SUCCESS;
    }

    queue__free(p);

    return SUCCESS;
}

int test_queue__is_empty_on_non_empty_queue()
{
    printf("%s... ", __func__);

    struct queue *p = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id card = CARD_MONASTERY_ALONE;
    queue__enqueue(p, &card);

    if (queue__is_empty(p)) {
        queue__free(p);
        return !SUCCESS;
    }

    queue__free(p);

    return SUCCESS;
}

int test_queue__enqueue_on_non_empty_queue()
{
    printf("%s... ", __func__);

    struct queue *p = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id card1 = CARD_MONASTERY_ALONE;
    enum card_id card2 = CARD_CITY_ALL_SIDES;
    enum card_id card3 = CARD_ROAD_STRAIGHT_CITY;
    enum card_id card4 = CARD_CITY_ONE_SIDE;
    queue__enqueue(p, &card1);
    queue__enqueue(p, &card2);
    queue__enqueue(p, &card3);
    queue__enqueue(p, &card4);
    queue__enqueue(p, &card1);

    if (queue__length(p) != 5) {
        queue__free(p);
        return !SUCCESS;
    }
    queue__free(p);
    return SUCCESS;
}

int test_queue_NULL()
{
    printf("%s... ", __func__);

    struct queue *p = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    queue__enqueue(p, NULL);

    if (!queue__is_empty(p)) {
        queue__free(p);
        return !SUCCESS;
    }

    queue__free(p);
    return SUCCESS;
}

int test_queue__enqueue_on_multiple_elements()
{
    printf("%s... ", __func__);

    struct queue *s = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id card = CARD_MONASTERY_ALONE;

    for (unsigned int i = 0; i < 500; i++) {
        queue__enqueue(s, &card);
    }

    if (queue__length(s) < 500) {
        queue__free(s);
        return !SUCCESS;
    }

    queue__free(s);

    return SUCCESS;
}

int test_queue__first()
{
    printf("%s... ", __func__);

    struct queue *s = queue__empty(&operator_copy, &operator_delete, &operator_debug);

    if (!queue__is_empty(s) || queue__first(s) != NULL) {
        queue__free(s);
        return !SUCCESS;
    }

    enum card_id card = CARD_CITY_THREE;
    queue__enqueue(s, &card);

    enum card_id *first = queue__first(s);
    if (queue__is_empty(s) ||  *first != CARD_CITY_THREE) {
        free(first);
        queue__free(s);
        return !SUCCESS;
    }
    free(first);
    queue__free(s);

    return SUCCESS;
}

int test_queue__dequeue_on_empty_queue()
{
    printf("%s... ", __func__);

    struct queue *s = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id *unqueueed = queue__dequeue(s);

    if (NULL != unqueueed) {
        queue__free(s);
        return !SUCCESS;
    }

    queue__free(s);
    return SUCCESS;
}

int test_queue__dequeue_on_non_empty_queue()
{
    printf("%s... ", __func__);

    struct queue *s = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id card1 = CARD_MONASTERY_ROAD;
    enum card_id card2 = CARD_MONASTERY_ALONE;
    queue__enqueue(s, &card1);
    queue__enqueue(s, &card2);
    enum card_id *unqueueed2 = queue__dequeue(s);
    enum card_id *unqueueed1 = queue__dequeue(s);


    if (!queue__is_empty(s)
            || (*unqueueed1 != CARD_MONASTERY_ROAD)
            || (*unqueueed2 != CARD_MONASTERY_ALONE)) {
        queue__free(s);
        free(unqueueed1);
        free(unqueueed2);
        return !SUCCESS;
    }

    free(unqueueed1);
    free(unqueueed2);
    queue__free(s);

    return SUCCESS;
}

int test_queue_length()
{
    printf("%s... ", __func__);

    struct queue *s = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id card1 = CARD_ROAD_TURN_RIGHT_CITY;
    enum card_id card2 = CARD_ROAD_TURN_LEFT_CITY;
    queue__enqueue(s, &card1);

    if (queue__length(s) != 1) {
        queue__free(s);
        return !SUCCESS;
    }

    queue__enqueue(s, &card2);

    if (queue__length(s) != 2) {
        queue__free(s);
        return !SUCCESS;
    }

    queue__free(s);

    return SUCCESS;
}

int test_queue_debug()
{
    printf("%s (expected 5 6 7 8 5)... ", __func__);

    struct queue *s = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id card1 = CARD_CITY_TUNNEL_SHLD;
    enum card_id card2 = CARD_CITY_TUNNEL;
    enum card_id card3 = CARD_PLAIN_TUNNEL;
    enum card_id card4 = CARD_PLAIN_TWO_CITIES;

    queue__enqueue(s, &card1);
    queue__enqueue(s, &card2);
    queue__enqueue(s, &card3);
    queue__enqueue(s, &card4);
    queue__enqueue(s, &card1);

    queue__debug(s);

    queue__free(s);

    return SUCCESS;
}


int main()
{
    printf("----------- TEST QUEUE -----------\n");

    print_success(test_queue__empty());
    print_success(test_queue__is_empty_on_empty_queue());
    print_success(test_queue__is_empty_on_non_empty_queue());
    print_success(test_queue__enqueue_on_non_empty_queue());
    print_success(test_queue_NULL());
    print_success(test_queue__enqueue_on_multiple_elements());
    print_success(test_queue__first());
    print_success(test_queue__dequeue_on_empty_queue());
    print_success(test_queue__dequeue_on_non_empty_queue());
    print_success(test_queue_length());
    print_success(test_queue_debug());

    return SUCCESS;
}
