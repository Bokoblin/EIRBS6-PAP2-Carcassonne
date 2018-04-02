#include <stdlib.h>
#include "test_utils.h"
#include "../src/common/board.h"

int test_init_board()
{
    printf("%s... ", __func__);

    struct board* b = init_board();
    free(b); // not to free just to show "make memory"

    //TODO : test_init_board

    return !SUCCESS;
}

int main()
{
    printf("----------- TEST STACK -----------\n");

    print_success(test_init_board());

    return SUCCESS;
}
