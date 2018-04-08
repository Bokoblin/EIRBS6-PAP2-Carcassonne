#include <stdlib.h>
#include <stdio.h>
#include "test_utils.h"
#include "../src/common/common_interface.h"
#include "../src/common/queue.h"

////////////////////////////////////////////////////////////////////
///     USER FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

void* operator_copy(void* given_card)
{
    if (given_card == NULL)
        return NULL;

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
    setvbuf (stdout, NULL, _IONBF, 0);
    printf("%d ", *_given_card);
}

////////////////////////////////////////////////////////////////////
///     queue FUNCTIONS TESTS
////////////////////////////////////////////////////////////////////


int test_queue__empty()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);

    if (q == NULL) {
        queue__free(q);
        return !SUCCESS;
    }

    queue__free(q);

    return SUCCESS;
}

int test_queue__is_empty_on_empty_queue()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);

    if (!queue__is_empty(q)) {
        queue__free(q);
        return !SUCCESS;
    }

    queue__free(q);

    return SUCCESS;
}

int test_queue__is_empty_on_non_empty_queue()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id card = CARD_MONASTERY_ALONE;
    
    if (queue__enqueue(q, &card) != SUCCESS) {
        queue__free(q);
        return !SUCCESS;
    }

    if (queue__is_empty(q)) {
        queue__free(q);
        return !SUCCESS;
    }

    queue__free(q);

    return SUCCESS;
}

int test_queue__enqueue_on_non_empty_queue()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id card1 = CARD_MONASTERY_ALONE;
    enum card_id card2 = CARD_CITY_ALL_SIDES;
    enum card_id card3 = CARD_ROAD_STRAIGHT_CITY;
    enum card_id card4 = CARD_CITY_ONE_SIDE;
    queue__enqueue(q, &card1);
    queue__enqueue(q, &card2);
    queue__enqueue(q, &card3);
    queue__enqueue(q, &card4);
    queue__enqueue(q, &card1);

    if (queue__length(q) != 5) {
        queue__free(q);
        return !SUCCESS;
    }
    queue__free(q);
    return SUCCESS;
}

int test_queue_NULL()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    queue__enqueue(q, NULL);

    if (!queue__is_empty(q)) {
        queue__free(q);
        return !SUCCESS;
    }

    queue__free(q);
    return SUCCESS;
}

int test_queue__enqueue_on_multiple_elements()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id card = CARD_MONASTERY_ALONE;

    for (unsigned int i = 0; i < 500; i++) {
        queue__enqueue(q, &card);
    }

    if (queue__length(q) < 500) {
        queue__free(q);
        return !SUCCESS;
    }

    queue__free(q);

    return SUCCESS;
}

int test_queue__first()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);

    if (!queue__is_empty(q) || queue__front(q) != NULL) {
        queue__free(q);
        return !SUCCESS;
    }

    enum card_id card = CARD_CITY_THREE;
    queue__enqueue(q, &card);

    enum card_id *first = queue__front(q);

    if (queue__is_empty(q) ||  *first != CARD_CITY_THREE) {
        free(first);
        queue__free(q);
        return !SUCCESS;
    }

    free(first);
    queue__free(q);

    return SUCCESS;
}

int test_queue__dequeue_on_empty_queue()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id *dequeued_e = queue__dequeue(q);

    if (dequeued_e != NULL) {
        free(dequeued_e);
        queue__free(q);
        return !SUCCESS;
    }

    queue__free(q);
    return SUCCESS;
}

int test_queue__dequeue_on_non_empty_queue()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id c1 = CARD_MONASTERY_ROAD;
    enum card_id c2 = CARD_MONASTERY_ALONE;

    queue__enqueue(q, &c1);
    queue__enqueue(q, &c2);

    enum card_id *dequeued_c1 = queue__dequeue(q); //FIXME: apparent memory error
    enum card_id *dequeued_c2 = queue__dequeue(q);

    if (!queue__is_empty(q)
            || (*dequeued_c1 != CARD_MONASTERY_ROAD) || (*dequeued_c2 != CARD_MONASTERY_ALONE)) {
        free(dequeued_c1);
        free(dequeued_c2);
        queue__free(q);
        return !SUCCESS;
    }

    free(dequeued_c1);
    free(dequeued_c2);
    queue__free(q);

    return SUCCESS;
}

int test_queue_length()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id card1 = CARD_ROAD_TURN_RIGHT_CITY;
    enum card_id card2 = CARD_ROAD_TURN_LEFT_CITY;
    queue__enqueue(q, &card1);

    if (queue__length(q) != 1) {
        queue__free(q);
        return !SUCCESS;
    }

    queue__enqueue(q, &card2);

    if (queue__length(q) != 2) {
        queue__free(q);
        return !SUCCESS;
    }

    queue__free(q);

    return SUCCESS;
}

int test_queue_debug()
{
    printf("%s (expected 5 6 7 8 5)... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id card1 = CARD_CITY_TUNNEL_SHLD;
    enum card_id card2 = CARD_CITY_TUNNEL;
    enum card_id card3 = CARD_PLAIN_TUNNEL;
    enum card_id card4 = CARD_PLAIN_TWO_CITIES;

    queue__enqueue(q, &card1);
    queue__enqueue(q, &card2);
    queue__enqueue(q, &card3);
    queue__enqueue(q, &card4);
    queue__enqueue(q, &card1);

    queue__debug(q);

    queue__free(q);

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
