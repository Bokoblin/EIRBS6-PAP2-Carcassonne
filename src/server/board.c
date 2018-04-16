#include <stdlib.h>
#include "board.h"
#include "../common/utils.h"
#include "../common/ADT/set.h"
#include "../server/meeple.h"
#include "../common/deck.h"
#include "player.h"
#include "function_pointers.h"

struct board *board__init(struct stack *drawing_stack)
{
    struct board *b = safe_malloc(sizeof(struct board));

    b->cards_set = set__empty(cards_set_copy_op, cards_set_delete_op, cards_set_compare_op);
    b->meeples_set = set__empty(meeples_set_copy_op, meeples_set_delete_op, meeples_set_compare_op);

    enum card_id *ci = stack__pop(drawing_stack);
    if (ci == NULL) {
        b->first_card = NULL;
    } else {
        b->first_card = card__init(*ci);
        free(ci);
        set__add(b->cards_set, b->first_card);
    }

    return b;
}

int board__add_card(struct board *b, struct card *c)
{
    if (b == NULL || c == NULL)
        return -1;

    //TODO: use or replace card__add_card()

    return 0;
}

void board__free(struct board *b)
{
    if (b->first_card != NULL)
        card__free(b->first_card);
    set__free(b->cards_set);
    set__free(b->meeples_set);
    free(b);
}
