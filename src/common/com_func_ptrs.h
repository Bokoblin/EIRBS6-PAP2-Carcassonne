#ifndef COMMON_FUNCTION_POINTERS_H
#define COMMON_FUNCTION_POINTERS_H

#include <stdio.h>
#include "../common/common_interface.h"
#include "../common/card.h"
#include "../common/meeple.h"

////////////////////////////////////////////////////////////////////
///     OPERATORS FOR MOVE ENUM
////////////////////////////////////////////////////////////////////

extern void* move_copy_op(struct move *m);
extern void move_delete_op(struct move *m);
int move_compare_op(struct move* m1, struct move* m2);
void move_debug_op(struct move *m);


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


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR AREA_TYPE ENUM
////////////////////////////////////////////////////////////////////

void* area_type_enum_copy_op(const enum area_type *area);
void area_type_enum_delete_op(enum area_type *area);
int area_type_enum_compare_op(const enum area_type *area1, const enum area_type *area2);
void area_type_enum_debug_op(const enum area_type *area);


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR PLACE ENUM
////////////////////////////////////////////////////////////////////

void* place_enum_copy_op(const enum place *pl);
void place_enum_delete_op(enum place *pl);
int place_enum_compare_op(const enum place *pl1, const enum place *pl2);
void place_enum_debug_op(const enum place *pl);

#endif
