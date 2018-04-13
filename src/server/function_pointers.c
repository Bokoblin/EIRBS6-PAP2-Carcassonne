#include "function_pointers.h"

////////////////////////////////////////////////////////////////////
///     OPERATORS FOR MOVES QUEUE
////////////////////////////////////////////////////////////////////

void* move_queue_copy_op(struct move* m)
{
    struct move *new_move = malloc(sizeof(struct move));
    new_move->card = m->card;
    new_move->check = m->check;
    new_move->dir = m->dir;
    new_move->onto = m->onto;
    new_move->place = m->place;
    new_move->player = m->player;

    return new_move;
}

void move_queue_delete_op(struct move* m)
{
    free(m);
}

void move_queue_debug_op(struct move* m)
{
    printf("=== Display move ===\n");
    printf("\tplayer id: %d\n", m->player);
    printf("\tcard id: %d\n", m->card);
    printf("\tposition: {%d, %d}\n", m->onto.x, m->onto.y);
    printf("\tdirection: %d\n", m->dir);
    printf("\tplace: %d\n", m->place);
}


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR PLAYERS QUEUE
////////////////////////////////////////////////////////////////////

void* player_queue_copy_op(struct player* p)
{
    struct player *new_p = malloc(sizeof(struct player));
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

void player_queue_delete_op(struct player* p)
{
    player__free(p);
}

void player_queue_debug_op(struct player* p)
{
    printf("Player #%d named %s\n", p->id, p->get_player_name());
}


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR CARDS SET
////////////////////////////////////////////////////////////////////

void* cards_set_copy_op(struct card* c)
{
    struct card *new_card = malloc(sizeof(struct card));
    new_card->type = c->type;
    new_card->orientation = c->orientation;

    for (int i = 0; i < DIRECTION_NUMBER; i++)
        new_card->neighbors[i] = c->neighbors[i];

    return new_card;
}

void cards_set_delete_op(struct card* c)
{
    free(c);
}

int cards_set_compare_op(struct card* c1, struct card* c2)
{
    if (c1->pos.x < c2->pos.x)
        return -1;
    else if ((c1->pos.x == c2->pos.x) && (c1->pos.y == c2->pos.y))
        return 0;
    else if (c1->pos.x == c2->pos.x)
        return (c1->pos.y < c2->pos.y) ? -1 : 1;
    else
        return 1;
}


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR MEEPLES SET
////////////////////////////////////////////////////////////////////

void* meeples_set_copy_op(struct meeple* m)
{
    struct meeple *new_meeple = malloc(sizeof(struct meeple));
    new_meeple->player_id = m->player_id;
    new_meeple->belonging_area = m->belonging_area;
    new_meeple->position = m->position;

    return new_meeple;
}

void meeples_set_delete_op(struct meeple* m)
{
    free(m);
}

int meeples_set_compare_op(struct meeple* m1, struct meeple* m2)
{
    if (m1->position < m2->position)
        return -1;
    else if (m1->position == m2->position)
        return 0;
    else
        return 1;
}
