#include "zone.h"

struct zone *zone__empty(enum area_type area)
{
    struct zone *z = safe_malloc(sizeof(struct zone));
    z->area_set = set__empty(area_type__copy_op, area_type__delete_op, area_type__compare_op, area_type__debug_op);
    z->meeple_set = set__empty(meeple_copy_op, meeple_delete_op, meeple_compare_op, meeple_debug_op);
    z->area = area;
    z->score = 0;
    z->holes = 0;

    return z;
}

struct set *zone__cards_available_zones(struct card *c)
{
    //allowed_positions gives already one enum place per zone
    struct set *p_place = set__empty(place__copy_op, place__delete_op, place__compare_op, place__debug_op);
    enum card_id id = c->type.id;
    int p = 0;
    while (allowed_positions[id][p] != NO_MEEPLE) {
        set__add(p_place, (void *) &allowed_positions[id][p]);
        p++;
    }

    return p_place;
}

enum area_type *zone__search_allowed_position_area(struct card *c, enum area_type *area)
{
    (void) c;
    (void) area;

    return NULL;
}

struct set *zone__matching_area_set(enum area_type *area)
{
    //TODO: search in the global set having all the zones where is the set having the area pointer into
    //TODO: before, search the area on the  card on the same zone which correspond to the "allowed position array"
    //      --> TODO: zone__search_allowed_position_area
    //TODO: add the card pointer having the area and the global set of zones as parameter
    // (the global set of zone should be in the "board" structure)
    struct set *area_set = set__empty(area_type__copy_op, area_type__delete_op, area_type__compare_op,
                                      area_type__debug_op);
    set__add(area_set, area);
    return area_set;
}

int zone__add_areas(struct zone *z, struct card *c, enum place p)
{
    //TODO: zone__add_areas
    //note: if you want to add only the area in the "allowed_positions",
    //just search it with zone__search_allowed_position_area and add it to the zone's set
    (void) z;
    (void) c;
    (void) p;
    return 0;
}

int *zone__is_fusion_required(struct card *c)
{
    //TODO: zone__is_fusion_required
    //Search into the global zone set if two zones  are linked with this card
    (void) c;

    return NULL;
}

int zone__fusion_zone(struct set *global_zone_set, struct zone *z1, struct zone *z2)
{
    assert_not_null(global_zone_set, __func__, "Global zone set");
    assert_not_null(z1, __func__, "Zone 1");
    assert_not_null(z2, __func__, "Zone 2");

    struct set *merged_area_set = set__fusion(z1->area_set, z2->area_set);
    if (merged_area_set == NULL)
        return !SUCCESS;

    struct set *merged_meeple_set = set__fusion(z1->meeple_set, z2->meeple_set);
    if (merged_meeple_set == NULL)
        return !SUCCESS;

    z1->area_set = merged_area_set;
    z1->meeple_set = merged_meeple_set;
    z1->score = z1->score + z2->score;
    z1->holes = z1->holes + z2->holes - 2; //-2 because of the link from the common card

    set__remove(global_zone_set, z2);

    return SUCCESS;
}

int zone__update()
{
    //TODO: zone__update
    return 0;
}

int zone__add_meeple(struct zone *z, struct meeple *m)
{
    if (!set__add_no_copy(z->meeple_set, m)) return SUCCESS;
    return !SUCCESS;
}

void zone__free(struct zone *z)
{
    set__free(z->area_set);
    set__free(z->meeple_set);
    free(z);
}
