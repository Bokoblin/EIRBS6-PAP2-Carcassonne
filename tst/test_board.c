#include <stdlib.h>
#include "test_utils.h"
#include "../src/server/board.h"
#include "../src/common/ADT/set.h"
#include "../src/common/utils.h"
#include "../src/server/function_pointers.h"

////////////////////////////////////////////////////////////////////
///     OPERATORS FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

void cards_set_debug_op(struct card* c)
{
    printf("\n=== Display card ===\n");
    printf("\ttype id: %d\n", c->type.id);
    printf("\tareas: { ");
    for (int i = 0; i < MAX_ZONES; i++) printf("%d, ", c->type.areas[i]);
    printf("}\n");
    printf("\tposition: { %d, %d }\n", c->pos.x, c->pos.y);
    printf("\tneighbours: { ");
    for (int i = 0; i < DIRECTION_NUMBER; i++)
        if (c->neighbors[i] != NULL)
            printf("%d, ", c->neighbors[i]->type.id);
    printf("}\n");
    printf("\torientation: %d\n", c->orientation);
}

////////////////////////////////////////////////////////////////////
///     BOARD FUNCTIONS TESTS
////////////////////////////////////////////////////////////////////

int test_board__empty()
{
    printf("%s... ", __func__);

    struct board* b1 = board__init(NULL);

    if (b1->first_card != NULL || !set__is_empty(b1->cards_set) || !set__is_empty(b1->meeples_set)) {
        board__free(b1);
        return !TEST_SUCCESS;
    }

    struct stack* s = stack__empty(cards_set_copy_op, cards_set_delete_op, cards_set_debug_op);
    struct card* c = card__init(CARD_PLAIN_CITY);
    stack__push(s, c);

    board__free(b1);
    b1 = board__init(s);

    if (b1->first_card == NULL || set__is_empty(b1->cards_set) || !set__is_empty(b1->meeples_set)) {
        board__free(b1);
        return !TEST_SUCCESS;
    }

    board__free(b1);

    return TEST_SUCCESS;
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
