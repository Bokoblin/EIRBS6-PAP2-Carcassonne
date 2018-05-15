#ifndef ZONE_H
#define ZONE_H

#include "ADT/set.h"
#include "area_type.h"
#include "card.h"
#include "meeple.h"
#include "utils.h"

#define MAX_ZONES 9

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

struct partition
{
    struct set * zones[MAX_ZONES];
    int size;
};

////////////////////////////////////////////////////////////////////
///     FUNCTIONS
////////////////////////////////////////////////////////////////////


/**
 * @brief Creates and initialises a new zone
 * 
 * @param area Is the area type of the zone
 * @return struct zone* 
 */
struct zone *zone__empty(enum area_type area);


/**
 * @brief Returns a set of pointers of enum place*
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
 * @param area is a pointer to the area in the enum area_type areas[MAX_ZONES] in the card
 * @return struct set* containing the area
 */
struct set *zone__matching_area_set(enum area_type *area);


/**
 * @brief Searches a pointer to the area in "allowed_positions" in the same zone as the given area pointer
 * 
 * @param c is the card where the researched is done
 * @param area is a pointer to one of the areas on the given card
 * @return enum area_type* a pointer to the position on the card having to same area as the area parameter,
 * and which is into the "allowed_positions" array according to the given card c
 */
enum area_type *zone__search_allowed_position_area(struct card *c, enum area_type *area);


/**
  *@brief gives the partition
  *
  *
  */
struct partition zone__zones_by_card_id(enum card_id id);

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
 * @brief Make a fusion of z1 and z2 into z1, and remove z2 from the global set.
 * Please note that the card making the fusion have to be already in one of the two zones
 * @return SUCCESS or FAILED (both z1 and z2 aren't modified if the fusion failed)
 */
int zone__fusion_zone(struct set *global_zone_set, struct zone *z1, struct zone *z2);


/**
 * @brief General call to the other functions to update a zone
 * @return
 */
int zone__update();

/**
 * @brief Adds a meeple to the zone
 * @param z is the zone where you want to place a meeple
 * @param m is the already-initialised meeple
 * @return SUCCESS if the meeple was added, FAILURE else
 */
int zone__add_meeple(struct zone *z, struct meeple *m);


/**
 * @brief Frees all the allocated memory in the zone
 * @param z is the zone you have to free
 */
void zone__free(struct zone *z);

#endif 
