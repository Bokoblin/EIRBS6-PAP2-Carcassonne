#include "meeple.h"
#include "utils.h"

struct meeple *meeple__init(unsigned int player_id, struct card *card, enum place place)
{
    if (place != NO_MEEPLE) {
        struct meeple *m = safe_malloc(sizeof(struct meeple));
        m->player_id = player_id;
        m->related_card = card;
        m->position = place;
        m->belonging_area = card__get_relative_area(card, place);
        return m;
    } else {
        return NULL;
    }
}

void meeple__free(struct meeple *m)
{
    free(m);
}


////////////////////////////////////////////////////////////////////
///     OPERATORS (COPY, DELETE, COMPARE, DEBUG)
////////////////////////////////////////////////////////////////////

void* meeple_copy_op(const struct meeple *m)
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

int meeple_compare_op(const struct meeple *m1, const struct meeple *m2)
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
