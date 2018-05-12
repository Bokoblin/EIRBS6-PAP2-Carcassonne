#include <string.h>
#include "com_func_ptrs.h"
#include "utils.h"

////////////////////////////////////////////////////////////////////
///     OPERATORS FOR MOVE STRUCT
////////////////////////////////////////////////////////////////////

void* move_copy_op(struct move *m)
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

void move_delete_op(struct move *m)
{
    free(m);
}

int move_compare_op(struct move* m1, struct move* m2)
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

void move_debug_op(struct move *m)
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

void* cardid_copy_op(const enum card_id *p_card_id)
{
    if (p_card_id == NULL)
        return NULL;

    enum card_id *new_card = safe_malloc(sizeof(enum card_id));
    *new_card = *p_card_id;
    return new_card;
}

void cardid_delete_op(enum card_id *p_card_id)
{
    free(p_card_id);
}

int cardid_compare_op(const enum card_id *ci1, const enum card_id *ci2)
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

void cardid_debug_op(const enum card_id *p_card_id)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (p_card_id == NULL)
        printf("NULL");
    else
        printf("Card id (%d)", *p_card_id);
}


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR CARD STRUCT
////////////////////////////////////////////////////////////////////

void* card_copy_op(struct card *c)
{
    assert_not_null(c, __func__, "c parameter");

    struct card *new_card = safe_malloc(sizeof(struct card));
    new_card->type = c->type;
    new_card->direction = c->direction;
    new_card->pos = c->pos;

    for (int i = 0; i < NB_DIRECTIONS; i++)
        new_card->neighbors[i] = c->neighbors[i];

    return new_card;
}

void card_delete_op(struct card *c)
{
    card__free(c);
}

int card_compare_op(struct card *c1, struct card *c2)
{
    assert_not_null(c1, __func__, "c1 parameter");
    assert_not_null(c2, __func__, "c2 parameter");

    if (c1->pos.x < c2->pos.x)
        return -1;
    else if ((c1->pos.x == c2->pos.x) && (c1->pos.y == c2->pos.y))
        return 0;
    else if (c1->pos.x == c2->pos.x)
        return (c1->pos.y < c2->pos.y) ? -1 : 1;
    else
        return 1;
}

void card_debug_op(const struct card *c)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (c == NULL)
        printf("NULL");
    else {
        printf("Card (type id: %d, areas: {", c->type.id);
        for (int i = 0; i < MAX_ZONES; i++) {
            if (i < MAX_ZONES-1)
                printf("%d, ", c->type.areas[i]);
            else
                printf("%d", c->type.areas[i]);
        }
        printf("}, direction: %d, pos: { %d, %d }, neighbours: {", c->direction, c->pos.x, c->pos.y);

        for (int i = 0; i < NB_DIRECTIONS; i++) {
            if (c->neighbors[i] != NULL) {
                if (i < NB_DIRECTIONS-1)
                    printf("\x1B[33m%p (%d)\x1B[0m, ", c->neighbors[i], c->neighbors[i]->type.id);
                else
                    printf("\x1B[33m%p (%d)\x1B[0m", c->neighbors[i], c->neighbors[i]->type.id);
            } else {
                if (i < NB_DIRECTIONS-1)
                    printf("\x1B[32mNULL\x1B[0m, ");
                else
                    printf("\x1B[32mNULL\x1B[0m");
            }
        }

        printf("})\n");
    }
}


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR MEEPLES SET
////////////////////////////////////////////////////////////////////

void* meeple_copy_op(struct meeple *m)
{
    assert_not_null(m, __func__, "m parameter");

    struct meeple *new_meeple = safe_malloc(sizeof(struct meeple));
    new_meeple->player_id = m->player_id;
    new_meeple->belonging_area = m->belonging_area;
    new_meeple->position = m->position;

    return new_meeple;
}

void meeple_delete_op(struct meeple *m)
{
    free(m);
}

int meeple_compare_op(struct meeple *m1, struct meeple *m2)
{
    assert_not_null(m1, __func__, "m1 parameter");
    assert_not_null(m2, __func__, "m2 parameter");

    if (m1->position < m2->position)
        return -1;
    else if (m1->position == m2->position)
        return 0;
    else
        return 1;
}

void meeple_debug_op(const struct meeple *m)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (m == NULL)
        printf("NULL");
    else
        printf("Meeple (player id: %d, pos: %d, area type: %d)", m->player_id, m->position, m->belonging_area);
}


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR AREA_TYPE ENUM
////////////////////////////////////////////////////////////////////

void* area_type_enum_copy_op(const enum area_type *area)
{
    if (area == NULL)
        return NULL;

    enum area_type *new_area_type = safe_malloc(sizeof(enum area_type));
    *new_area_type = *area;
    return new_area_type;
}

void area_type_enum_delete_op(enum area_type *area)
{
    free(area);
}

int area_type_enum_compare_op(const enum area_type *area1, const enum area_type *area2)
{
    assert_not_null(area1, __func__, "area1 parameter");
    assert_not_null(area2, __func__, "area2 parameter");

    if (area1 < area2)
        return -1;
    else if (area1 == area2)
        return 0;
    else
        return 1;
}

void area_type_enum_debug_op(const enum area_type *area)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (area == NULL)
        printf("NULL");
    else
        printf("area type (%d)", *area);
}


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR PLACE ENUM
////////////////////////////////////////////////////////////////////

void* place_enum_copy_op(const enum place *pl)
{
    if (pl == NULL)
        return NULL;

    enum place *new_place = safe_malloc(sizeof(enum place));
    *new_place = *pl;
    return new_place;
}

void place_enum_delete_op(enum place *pl)
{
    free(pl);
}

int place_enum_compare_op(const enum place *pl1, const enum place *pl2)
{
    assert_not_null(pl1, __func__, "pl1 parameter");
    assert_not_null(pl2, __func__, "pl2 parameter");

    if (pl1 < pl2)
        return -1;
    else if (pl1 == pl2)
        return 0;
    else
        return 1;
}

void place_enum_debug_op(const enum place *pl)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (pl == NULL)
        printf("NULL");
    else
        printf("emum place (%d)", *pl);
}
