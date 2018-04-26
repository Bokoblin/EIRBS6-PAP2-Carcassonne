#ifndef SERVER_FUNCTION_POINTERS_H
#define SERVER_FUNCTION_POINTERS_H

#include "../server/player.h"

////////////////////////////////////////////////////////////////////
///     OPERATORS FOR PLAYER STRUCT
////////////////////////////////////////////////////////////////////

void* player_copy_op(const struct player *p);
void player_delete_op(struct player *p);
int player_compare_op(const struct player *p1, const struct player *p2);
void player_debug_op(const struct player *p);

#endif
