#include "common_tests_utils.h"
#include "../src/server/board.h"
#include "../src/common/ADT/set.h"
#include "../src/common/deck.h"


int test_client__init()
{
    printf("%s... ", __func__);
    printf("NOT DONE YET - ");
    return !TEST_SUCCESS;
}

int test_client__free()
{
    printf("%s... ", __func__);
    printf("NOT DONE YET - ");
    return !TEST_SUCCESS;
}

int test_update_board()
{
    printf("%s... ", __func__);
    printf("NOT DONE YET - ");
    return !TEST_SUCCESS;
}

int test_play_card()
{
    printf("%s... ", __func__);
    printf("NOT DONE YET - ");
    return !TEST_SUCCESS;
}


int main()
{
    printf("----------- TEST CLIENT -----------\n");

    int nb_success = 0;
    int nb_tests = 0;

    print_test_result(test_client__init(), &nb_success, &nb_tests);
    print_test_result(test_client__free(), &nb_success, &nb_tests);
    print_test_result(test_update_board(), &nb_success, &nb_tests);
    print_test_result(test_play_card(), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
