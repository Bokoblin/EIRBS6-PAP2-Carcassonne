#include "server_func_ptrs.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////
///     OPERATORS FOR PLAYER STRUCT
////////////////////////////////////////////////////////////////////

void* player_copy_op(const struct player *p)
{
    assert_not_null(p, __func__, "p parameter");

    struct player *new_p = safe_malloc(sizeof(struct player));
    new_p->id = p->id;
    new_p->score = p->score;
    new_p->nb_meeples = p->nb_meeples;
    new_p->lib_ptr = p->lib_ptr;
    new_p->get_player_name = p->get_player_name;
    new_p->initialize = p->initialize;
    new_p->play = p->play;
    new_p->finalize = p->finalize;

    return new_p;
}

void player_delete_op(struct player *p)
{
    player__free(p);
}

int player_compare_op(const struct player *p1, const struct player *p2)
{
    assert_not_null(p1, __func__, "p1 parameter");
    assert_not_null(p2, __func__, "p2 parameter");

    if (p1->id < p2->id)
        return -1;
    else if (p1->id == p2->id)
        return 0;
    else
        return 1;
}

void player_debug_op(const struct player *p)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (p == NULL)
        printf("NULL");
    else
        printf("Player (id: %d, name: %s)\n", p->id, p->get_player_name());
}
