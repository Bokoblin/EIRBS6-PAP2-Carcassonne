#include <stdlib.h>
#include "test_utils.h"
#include "../src/common/board.h"

int test_board__empty()
{
    printf("%s... ", __func__);

    struct board* b1 = board__empty();

    if (board__first_card(b1)->type.id != CARD_JUNCTION_CITY) {  //FIXME: check if it's that
        board__free(b1);
        return !SUCCESS;
    }

    if (b1->free_positions_array == NULL) {
        board__free(b1);
        return !SUCCESS;
    }

    board__free(b1);

    return SUCCESS;
}

int test_board__add_card()
{
    printf("%s... ", __func__);

    //TODO : test_board__add_card

    printf("- Not tested yet - ");

    return !SUCCESS;
}

int main()
{
    printf("----------- TEST BOARD -----------\n");

    print_success(test_board__empty());
    print_success(test_board__add_card());

    return SUCCESS;
}
