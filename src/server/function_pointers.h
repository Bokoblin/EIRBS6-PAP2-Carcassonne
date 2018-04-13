#ifndef SERVER_FUNCTION_POINTERS_H
#define SERVER_FUNCTION_POINTERS_H

#include <stdio.h>
#include "../common/common_interface.h"
#include "../server/player.h"
#include "../common/card.h"
#include "../server/meeple.h"

////////////////////////////////////////////////////////////////////
///     OPERATORS FOR MOVES QUEUE
////////////////////////////////////////////////////////////////////

extern void* move_queue_copy_op(struct move* m);
extern void move_queue_delete_op(struct move* m);
void move_queue_debug_op(struct move* m);


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR PLAYERS QUEUE
////////////////////////////////////////////////////////////////////

void* player_queue_copy_op(struct player* p);
void player_queue_delete_op(struct player* p);
void player_queue_debug_op(struct player* p);


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR CARDS SET
////////////////////////////////////////////////////////////////////

void* cards_set_copy_op(struct card* c);
void cards_set_delete_op(struct card* c);
int cards_set_compare_op(struct card* c1, struct card* c2);


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR MEEPLES SET
////////////////////////////////////////////////////////////////////

void* meeples_set_copy_op(struct meeple* m);
void meeples_set_delete_op(struct meeple* m);
int meeples_set_compare_op(struct meeple* m1, struct meeple* m2);

#endif
