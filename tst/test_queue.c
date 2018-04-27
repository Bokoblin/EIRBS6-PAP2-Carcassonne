#include "common_tests_utils.h"
#include "../src/common/ADT/queue.h"
#include "../src/common/utils.h"

int test_queue__empty()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);

    if (q == NULL) {
        queue__free(q);
        return !TEST_SUCCESS;
    }
    queue__free(q);

    return TEST_SUCCESS;
}

int test_queue__is_empty_on_empty_queue()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);

    if (!queue__is_empty(q)) {
        queue__free(q);
        return !TEST_SUCCESS;
    }

    queue__free(q);

    return TEST_SUCCESS;
}

int test_queue__is_empty_on_non_empty_queue()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id card = CARD_MONASTERY_ALONE;

    if (queue__enqueue(q, &card) != TEST_SUCCESS) {
        queue__free(q);
        return !TEST_SUCCESS;
    }

    if (queue__is_empty(q)) {
        queue__free(q);
        return !TEST_SUCCESS;
    }

    queue__free(q);

    return TEST_SUCCESS;
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
        return !TEST_SUCCESS;
    }
    queue__free(q);
    return TEST_SUCCESS;
}

int test_queue_NULL()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    queue__enqueue(q, NULL);

    if (!queue__is_empty(q)) {
        queue__free(q);
        return !TEST_SUCCESS;
    }

    queue__free(q);
    return TEST_SUCCESS;
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
        return !TEST_SUCCESS;
    }

    queue__free(q);

    return TEST_SUCCESS;
}

int test_queue__front()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);

    enum card_id* front  = queue__front(q);

    if (!queue__is_empty(q) || front != NULL) {
        free(front);
        queue__free(q);
        return !TEST_SUCCESS;
    }

    free(front);

    enum card_id card = CARD_CITY_THREE;
    queue__enqueue(q, &card);

    front = queue__front(q);

    if (queue__is_empty(q) || *front != CARD_CITY_THREE) {
        free(front);
        queue__free(q);
        return !TEST_SUCCESS;
    }

    free(front);
    queue__free(q);

    return TEST_SUCCESS;
}

int test_queue__back()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);

    enum card_id* back = queue__back(q);

    if (!queue__is_empty(q) || back != NULL) {
        free(back);
        queue__free(q);
        return !TEST_SUCCESS;
    }

    free(back);

    enum card_id card1 = CARD_PLAIN_CITY_SHLD;
    enum card_id card2 = CARD_PLAIN_CITY_ROAD_SHLD;
    queue__enqueue(q, &card1);
    queue__enqueue(q, &card2);

    back = queue__back(q);

    if (queue__is_empty(q) || *back != CARD_PLAIN_CITY_ROAD_SHLD) {
        free(back);
        queue__free(q);
        return !TEST_SUCCESS;
    }

    free(back);
    queue__free(q);

    return TEST_SUCCESS;
}

int test_queue__dequeue_on_empty_queue()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id *front = queue__front(q);
    queue__dequeue(q);

    if (front != NULL) {
        free(front);
        queue__free(q);
        return !TEST_SUCCESS;
    }

    free(front);
    queue__free(q);
    return TEST_SUCCESS;
}

int test_queue__dequeue_on_non_empty_queue()
{
    printf("%s... ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id c1 = CARD_MONASTERY_ROAD;
    enum card_id c2 = CARD_MONASTERY_ALONE;

    queue__enqueue(q, &c1);
    queue__enqueue(q, &c2);

    enum card_id *front_c1 = queue__front(q);
    queue__dequeue(q);
    enum card_id *front_c2 = queue__front(q);
    queue__dequeue(q);

    if (!queue__is_empty(q)
            || (*front_c1 != CARD_MONASTERY_ROAD)
            || (*front_c2 != CARD_MONASTERY_ALONE)) {
        free(front_c1);
        free(front_c2);
        queue__free(q);
        return !TEST_SUCCESS;
    }

    free(front_c1);
    free(front_c2);
    queue__free(q);

    return TEST_SUCCESS;
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
        return !TEST_SUCCESS;
    }

    queue__enqueue(q, &card2);

    if (queue__length(q) != 2) {
        queue__free(q);
        return !TEST_SUCCESS;
    }

    queue__free(q);

    return TEST_SUCCESS;
}

int test_queue_debug()
{
    printf("%s: ", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id card1 = CARD_CITY_TUNNEL_SHLD;
    enum card_id card2 = CARD_CITY_TUNNEL;
    enum card_id card3 = CARD_PLAIN_TUNNEL;
    enum card_id card4 = CARD_PLAIN_TWO_CITIES;

    queue__enqueue(q, &card1);
    queue__enqueue(q, &card2);
    queue__dequeue(q);

    printf("\nExpected: (6) and (NULL 6)\n");

    queue__debug(q, false);

    queue__enqueue(q, &card3);
    queue__enqueue(q, &card4);

    printf("\nExpected: (6 7 8) and (NULL 6 7 8)\n");

    queue__debug(q, false);

    queue__free(q);

    printf("%s... ", __func__);
    return TEST_SUCCESS;
}

int test_queue__enqueue_and_dequeue_on_multiple_elements_with_debug()
{
    printf("%s: \n", __func__);

    struct queue *q = queue__empty(&operator_copy, &operator_delete, &operator_debug);
    enum card_id card = CARD_MONASTERY_ALONE;

    for (unsigned int i = 0; i < 60; i++) {
        queue__enqueue(q, &card);
    }

    printf("Added 60 elements, expected capacity of 64 and size of 60\n");

    if (queue__length(q) < 60) {
        queue__free(q);
        return !TEST_SUCCESS;
    }

    queue__debug(q, false);

    for (unsigned int i = 0; i < 40; i++) {
        queue__dequeue(q);
    }

    printf("Removed 40 elements, expected capacity of 32 and size of 20\n");

    if (queue__length(q) < 20) {
        queue__free(q);
        return !TEST_SUCCESS;
    }

    queue__debug(q, false);

    for (unsigned int i = 0; i < 20; i++) {
        queue__enqueue(q, &card);
    }

    printf("Added 20 elements, expected capacity of 64 and size of 40\n");

    if (queue__length(q) < 40) {
        queue__free(q);
        return !TEST_SUCCESS;
    }

    queue__debug(q, false);

    for (unsigned int i = 0; i < 30; i++) {
        queue__dequeue(q);
    }

    printf("Removed 30 elements, expected capacity of 16 and size of 10\n");

    if (queue__length(q) < 10) {
        queue__free(q);
        return !TEST_SUCCESS;
    }

    queue__debug(q, false);

    queue__free(q);

    printf("%s... ", __func__);
    return TEST_SUCCESS;
}


int main()
{
    printf("----------- TEST QUEUE -----------\n");

    int nb_success = 0;
    int nb_tests = 0;

    print_test_result(test_queue__empty(), &nb_success, &nb_tests);
    print_test_result(test_queue__is_empty_on_empty_queue(), &nb_success, &nb_tests);
    print_test_result(test_queue__is_empty_on_non_empty_queue(), &nb_success, &nb_tests);
    print_test_result(test_queue__enqueue_on_non_empty_queue(), &nb_success, &nb_tests);
    print_test_result(test_queue_NULL(), &nb_success, &nb_tests);
    print_test_result(test_queue__enqueue_on_multiple_elements(), &nb_success, &nb_tests);
    print_test_result(test_queue__front(), &nb_success, &nb_tests);
    print_test_result(test_queue__back(), &nb_success, &nb_tests);
    print_test_result(test_queue__dequeue_on_empty_queue(), &nb_success, &nb_tests);
    print_test_result(test_queue__dequeue_on_non_empty_queue(), &nb_success, &nb_tests);
    print_test_result(test_queue_length(), &nb_success, &nb_tests);
    print_test_result(test_queue_debug(), &nb_success, &nb_tests);
    print_test_result(test_queue__enqueue_and_dequeue_on_multiple_elements_with_debug(), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
