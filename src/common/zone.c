#include "zone.h"

struct zone *zone__empty(enum area_type area)
{
    struct zone *z = safe_malloc(sizeof(struct zone));
    z->area_set = set__empty(area_type_enum_copy_op, area_type_enum_delete_op,
            area_type_enum_compare_op, area_type_enum_debug_op);
    z->meeple_set = set__empty(meeple_copy_op, meeple_delete_op, meeple_compare_op, meeple_debug_op);
    z->area = area;
    z->score = 0;
    z->holes = 0;

    return z;
}

struct set *zone__cards_available_zones(struct card *c)
{   //allowed_positions gives already one enum place per zone
    struct set *p_place = set__empty(place_enum_copy_op, place_enum_delete_op, place_enum_compare_op, place_enum_debug_op);
    enum card_id id = c->type.id;
    int p = 0;
    while (allowed_positions[id][p] != NO_MEEPLE) {
        set__add(p_place, &allowed_positions[id][p]);
        p++;    
    }
    
    return p_place;
}

struct set *zone__matching_area_set(enum area_type *area)
{
    (void*) area;
    return NULL;
}

int zone__add_areas(struct zone *z, struct card *c, enum place p)
{
    //TODO: zone__add_areas
    (void) z;
    (void) c;
    (void) p;
    return 0;
}

int *zone__is_fusion_required(struct card *c)
{
    //TODO: zone__is_fusion_required
    (void) c;

    return NULL;
}

struct zone *zone__fusion_zone(struct zone *z1, struct zone *z2)
{
    //TODO: zone__fusion_zone
    (void) z1;
    (void) z2;

    return NULL;
}

int zone__update()
{
    //TODO: zone__update
    return 0;
}

int zone__add_meeple(struct zone *z, struct meeple *m)
{
    //TODO: zone__add_meeple
    (void) z;
    (void) m;
    return 0;
}

void zone__free(struct zone *z)
{
    set__free(z->area_set);
    set__free(z->meeple_set);
    free(z);
}
