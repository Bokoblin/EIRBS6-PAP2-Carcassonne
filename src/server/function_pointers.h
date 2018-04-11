#ifndef SERVER_FUNCTION_POINTERS_H
#define SERVER_FUNCTION_POINTERS_H

#include <stdio.h>
#include "../common/common_interface.h"
#include "../server/player.h"

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

#endif
