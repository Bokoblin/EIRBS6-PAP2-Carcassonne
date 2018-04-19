#ifndef SERVER_FUNCTION_POINTERS_H
#define SERVER_FUNCTION_POINTERS_H

#include <stdio.h>
#include "../common/common_interface.h"
#include "../server/player.h"
#include "../common/card.h"
#include "../server/meeple.h"

////////////////////////////////////////////////////////////////////
///     OPERATORS FOR MOVE ENUM
////////////////////////////////////////////////////////////////////

extern void* move_copy_op(struct move *m);
extern void move_delete_op(struct move *m);
int move_compare_op(struct move* m1, struct move* m2);
void move_debug_op(struct move *m);


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR PLAYER STRUCT
////////////////////////////////////////////////////////////////////

void* player_copy_op(const struct player *p);
void player_delete_op(struct player *p);
int player_compare_op(const struct player *p1, const struct player *p2);
void player_debug_op(const struct player *p);


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR CARD_ID ENUM
////////////////////////////////////////////////////////////////////

void* cardid_copy_op(const enum card_id *p_card_id);
void cardid_delete_op(enum card_id *p_card_id);
int cardid_compare_op(const enum card_id *ci1, const enum card_id *ci2);
void cardid_debug_op(const enum card_id *p_card_id);


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR CARD STRUCT
////////////////////////////////////////////////////////////////////

void* card_copy_op(struct card *c);
void card_delete_op(struct card *c);
int card_compare_op(struct card *c1, struct card *c2);
void card_debug_op(const struct card *c);


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR MEEPLE STRUCT
////////////////////////////////////////////////////////////////////

void* meeple_copy_op(struct meeple *m);
void meeple_delete_op(struct meeple *m);
int meeple_compare_op(struct meeple *m1, struct meeple *m2);
void meeple_debug_op(const struct meeple *m);

#endif
