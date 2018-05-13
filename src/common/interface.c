#include "interface.h"
#include "utils.h"

////////////////////////////////////////////////////////////////////
///     OPERATORS FOR MOVE STRUCT
////////////////////////////////////////////////////////////////////

void* move__copy_op(const struct move *m)
{
    assert_not_null(m, __func__, "m parameter");

    struct move *new_move = safe_malloc(sizeof(struct move));
    new_move->card = m->card;
    new_move->check = m->check;
    new_move->dir = m->dir;
    new_move->onto = m->onto;
    new_move->place = m->place;
    new_move->player = m->player;

    return new_move;
}

void move__delete_op(struct move *m)
{
    free(m);
}

int move__compare_op(const struct move *m1, const struct move *m2)
{
    assert_not_null(m1, __func__, "m1 parameter");
    assert_not_null(m2, __func__, "m2 parameter");

    if ((m1->onto.x == m2->onto.x) && (m1->onto.y == m2->onto.y)) {
        if (m1->dir == m2->dir)
            return 0;
        else
            return (m1->dir < m2->dir) ? -1 : 1;;
    }
    else if (m1->onto.x == m2->onto.x)
        return (m1->onto.y < m2->onto.y) ? -1 : 1;
    else
        return (m1->onto.x < m2->onto.x) ? -1 : 1;
}

void move__debug_op(const struct move *m)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (m == NULL)
        printf("NULL");
    else
        printf("Move (player id: %d, card id: %d, card pos: {%d, %d}, card direction: %d, meeple place: %d)\n",
               m->player, m->card, m->onto.x, m->onto.y, m->dir, m->place);
}


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR CARD_ID ENUM
////////////////////////////////////////////////////////////////////

void* card_id__copy_op(const enum card_id *ci)
{
    if (ci == NULL)
        return NULL;

    enum card_id *new_card = safe_malloc(sizeof(enum card_id));
    *new_card = *ci;
    return new_card;
}

void card_id__delete_op(enum card_id *ci)
{
    free(ci);
}

int card_id__compare_op(const enum card_id *ci1, const enum card_id *ci2)
{
    assert_not_null(ci1, __func__, "ci1 parameter");
    assert_not_null(ci2, __func__, "ci2 parameter");

    if (ci1 < ci2)
        return -1;
    else if (ci1 == ci2)
        return 0;
    else
        return 1;
}

void card_id__debug_op(const enum card_id *ci)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (ci == NULL)
        printf("NULL");
    else
        printf("Card id (%d)", *ci);
}


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR PLACE ENUM
////////////////////////////////////////////////////////////////////

void* place__copy_op(const enum place *p)
{
    if (p == NULL)
        return NULL;

    enum place *new_place = safe_malloc(sizeof(enum place));
    *new_place = *p;
    return new_place;
}

void place__delete_op(enum place *p)
{
    free(p);
}

int place__compare_op(const enum place *p1, const enum place *p2)
{
    assert_not_null(p1, __func__, "p1 parameter");
    assert_not_null(p2, __func__, "p2 parameter");

    if (p1 < p2)
        return -1;
    else if (p1 == p2)
        return 0;
    else
        return 1;
}

void place__debug_op(const enum place *p)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (p == NULL)
        printf("NULL");
    else
        printf("Place (%d)", *p);
}
