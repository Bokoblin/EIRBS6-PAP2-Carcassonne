#include <stdlib.h>
#include "test_utils.h"
#include "../src/server/board.h"

int test_board__empty()
{
    printf("%s... ", __func__);

    struct board* b1 = board__init(NULL);

    //if (b1->first_card->type.id != CARD_JUNCTION_CITY) {
    //    board__free(b1);
    //    return !SUCCESS;
    //}

    //TODO : test_board__empty (unfinished)

    board__free(b1);

    printf("- Not finished yet - ");

    return !TEST_SUCCESS;
}

int test_board__add_card()
{
    printf("%s... ", __func__);

    //TODO : test_board__add_card

    printf("- Not tested yet - ");

    return !TEST_SUCCESS;
}

int main()
{
    printf("----------- TEST BOARD -----------\n");

    print_success(test_board__empty());
    print_success(test_board__add_card());

    return TEST_SUCCESS;
}
