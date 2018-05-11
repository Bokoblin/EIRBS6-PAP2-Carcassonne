#include "zone.h"

struct zone *zone__empty(enum area_type area)
{
    struct zone *z = safe_malloc(sizeof(struct zone));
    z->area_set = set__empty(area_type_enum_copy_op, area_type_enum_delete_op, area_type_enum_compare_op, area_type_enum_debug_op);
    z->meeple_set = set__empty(meeple_copy_op, meeple_delete_op, meeple_compare_op, meeple_debug_op);
    z->area = area;
    z->score = 0;
    z->holes = 0;
    return z;
}

