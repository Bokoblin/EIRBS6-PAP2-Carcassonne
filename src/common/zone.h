#ifndef ZONE_H
#define ZONE_H

#include "ADT/set.h"
#include "card.h"
#include "meeple.h"
#include "area_type.h"
#include "utils.h"
#include "com_func_ptrs.h"

////////////////////////////////////////////////////////////////////
///     STRUCTURE
////////////////////////////////////////////////////////////////////

/**
 * @brief A structure containing every zones (roads, cities, etc.)
 */
struct zone
{
    struct set *area_set;
    struct set *meeple_set;
    enum area_type area;
    unsigned int score;
    unsigned int holes;
};


////////////////////////////////////////////////////////////////////
///     FUNCTIONS
////////////////////////////////////////////////////////////////////


/**
 * @brief Creates and initialise a new zone
 * 
 * @param area_type Is the area type of the zone
 * @return struct zone* 
 */
struct zone *zone__empty(enum area_type area);


/**
 * @brief Returns a set of pointers to enum place*
 * Only one enum place by zone is required
 * (advice: use the allowed_positions)
 * 
 * @param c is the considered card
 * @return struct set* of enum place*
 */
struct set *zone__cards_available_zones(struct card *c);


/**
 * @brief Returns the set containing the given area
 * 
 * @param area_type is a pointer to the area in the enum area_type areas[MAX_ZONES] in the card
 * @return struct set* containing the area
 */
struct set *zone__matching_area_set(enum area_type *area);


/**
 * @brief Adds all the areas on the card that makes a same zone
 * @note also update the score and the holes numbers on the given struct zone
 * 
 * @param z in where you want to add the areas
 * @param c is the card containing the areas
 * @param p is one of the areas making a zone
 * @return SUCCESS if the areas were added, FAILURE else
 */
int zone__add_areas(struct zone *z, struct card *c, enum place p);


/**
 * @brief Verify if a fusion is required
 * 
 * @details Verify foreach neighbor of the card if two different zones are linked.
 * If yes, returns a set of zones that must be merged
 * 
 * @param c is the card you have to check the neighbor from
 * @return int: 1 if the fusion is required, 0 if not
 */
int *zone__is_fusion_required(struct card *c);


/**
 * @brief Make a fusion of z1 and z2
 * @return a new zone which is the fusion of z1 and z2
 */
struct zone *zone__fusion_zone(struct zone *z1, struct zone *z2);


/**
 * @brief usage ??
 * @return
 */
int zone__update();

/**
 * @brief Adds a meeple to the zone
 * @param z is the zone where you want to place a meeple
 * @param m is the already-initialised meeple
 * @return SUCCESS if the areas were added, FAILURE else
 */
int zone__add_meeple(struct zone *z, struct meeple *m);


/**
 * @brief Frees all the allocated memory in the zone
 * @param z is the zone you have to free
 */
void zone__free(struct zone *z);

#endif 
