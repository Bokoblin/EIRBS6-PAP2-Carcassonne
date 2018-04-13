#ifndef CARDS_H
#define CARDS_H

#include "common_interface.h"
#include "stack.h"
#include "area_type.h"

#define DIRECTION_NUMBER 4
#define MAX_ZONES (LAST_POS-1)
#define DEFAULT_ORIENTATION NORTH_TO_NORTH

////////////////////////////////////////////////////////////////////
///     STRUCTURE
////////////////////////////////////////////////////////////////////


/**
 * @brief card_type
 * Describes the card of that type
 */
struct card_type
{
  enum card_id id;
  enum area_type areas[MAX_ZONES]; // indexed by enum place 
};

/**
 * @brief orientation
 * Lists all the possible orientations of the card comparing to its card_type
 */
enum orientation 
{
  NORTH_TO_NORTH, // the north of card_type coincide with the north of the card
  NORTH_TO_WEST,  // the north of card_type coincide with the west of the card
  NORTH_TO_SOUTH, // the north of card_type coincide with the south of the card
  NORTH_TO_EAST   // the north of card_type coincide with the east of the card
};

/**
 * @brief card
 * Gives the type of the card et its orientation
 */
struct card
{
  struct card_type type;
  struct card * neighbors[DIRECTION_NUMBER]; // Indexed by enum direction
  enum orientation orientation; 
};

// card types

static struct card_type ct_CARD_MONASTERY_ROAD = {CARD_MONASTERY_ROAD, {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, ROAD, FIELD, FIELD, FIELD, ABBEY}};
static struct card_type ct_CARD_MONASTERY_ALONE = {CARD_MONASTERY_ALONE, {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, ABBEY}};
static struct card_type ct_CARD_CITY_ALL_SIDES = {CARD_CITY_ALL_SIDES, {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, CITY}};
static struct card_type ct_CARD_ROAD_STRAIGHT_CITY = {CARD_ROAD_STRAIGHT_CITY, {CITY, CITY, CITY, FIELD, ROAD, FIELD, FIELD, FIELD, FIELD, FIELD, ROAD, FIELD, ROAD}};
static struct card_type ct_CARD_CITY_ONE_SIDE = {CARD_CITY_ONE_SIDE, {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, CITY, CITY, CITY, FIELD}};
static struct card_type ct_CARD_CITY_TUNNEL_SHLD = {CARD_CITY_TUNNEL_SHLD, {CITY, CITY, CITY, FIELD, FIELD, FIELD, CITY, CITY, CITY, FIELD, FIELD, FIELD, CITY}};
static struct card_type ct_CARD_PLAIN_TUNNEL = {CARD_PLAIN_TUNNEL, {CITY, CITY, CITY, FIELD, FIELD, FIELD, CITY, CITY, CITY, FIELD, FIELD, FIELD, FIELD}};
static struct card_type ct_CARD_PLAIN_TWO_CITIES = {CARD_PLAIN_TWO_CITIES, {CITY, CITY, CITY, CITY, CITY, CITY, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD}};
static struct card_type ct_CARD_ROAD_TURN_RIGHT_CITY = {CARD_ROAD_TURN_RIGHT_CITY, {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, FIELD}};
static struct card_type ct_CARD_ROAD_TURN_LEFT_CITY = {CARD_ROAD_TURN_LEFT_CITY, {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, FIELD, FIELD, CITY, CITY, CITY, FIELD}};
static struct card_type ct_CARD_JUNCTION_CITY = {CARD_JUNCTION_CITY, {CITY, CITY, CITY, FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, INTERSECTION}};
static struct card_type ct_CARD_PLAIN_CITY_SHLD = {CARD_PLAIN_CITY_SHLD, {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, FIELD}};
static struct card_type ct_CARD_PLAIN_CITY = {CARD_PLAIN_CITY, {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, ROAD}};
static struct card_type ct_CARD_PLAIN_CITY_ROAD_SHLD = {CARD_PLAIN_CITY_ROAD_SHLD, {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, FIELD}};
static struct card_type ct_CARD_PLAIN_CITY_ROAD = {CARD_PLAIN_CITY_ROAD, {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, FIELD}};
static struct card_type ct_CARD_CITY_THREE_SHLD = {CARD_CITY_THREE_SHLD, {CITY, CITY, CITY, FIELD, FIELD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, CITY}};
static struct card_type ct_CARD_CITY_THREE = {CARD_CITY_THREE, {CITY, CITY, CITY, FIELD, FIELD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, CITY}};
static struct card_type ct_CARD_CITY_THREE_ROAD_SHLD = {CARD_CITY_THREE_ROAD_SHLD, {CITY, CITY, CITY, FIELD, ROAD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, CITY}};
static struct card_type ct_CARD_CITY_THREE_ROAD = {CARD_CITY_THREE_ROAD, {CITY, CITY, CITY, FIELD, ROAD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, CITY}};
static struct card_type ct_CARD_ROAD_STRAIGHT = {CARD_ROAD_STRAIGHT, {FIELD, FIELD, FIELD, FIELD, ROAD, FIELD, FIELD, FIELD, FIELD, FIELD, ROAD, FIELD, ROAD}};
static struct card_type ct_CARD_ROAD_TURN = {CARD_ROAD_TURN, {FIELD, FIELD, FIELD, FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, FIELD, FIELD, ROAD}};
static struct card_type ct_CARD_JUNCTION_THREE = {CARD_JUNCTION_THREE, {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, FIELD, FIELD, INTERSECTION}};
static struct card_type ct_CARD_JUNCTION_FOUR = {CARD_JUNCTION_FOUR, {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, INTERSECTION}};




////////////////////////////////////////////////////////////////////
///     COMMON FUNCTIONS
////////////////////////////////////////////////////////////////////

/**
 * @brief Initializes a card with the type card_type
 * @param card_type pointer towards the type of card wanted 
 * @return a pointer towards a newly created card
 */
struct card* card__empty(struct card_type card_type);


/**
 * @brief Release memory allocated to the card structure
 * @param card the card to free
 */
void card__free(struct card *card);


/**
 * @brief get the area type 
 * @param card 
 * @param place the zone that we want to get
 * @return the area_type of the area place
 */
enum area_type card__get_area(struct card *card, enum place place);


/**
 * @brief decide if two cards match in a certain direction
 * @param card_1
 * @param card_2
 * @param direction the direction in which the cards are compared
 * @return 1 if the cards match, 0 otherwise
 */
int card__are_matching_direction(struct card *card_1, struct card *card_2, enum direction direction);


/**
 * @brief link two cards to each other in a certain direction
 * @param card_1
 * @param card_2
 * @param direction the direction in which the cards match (relative to card_1)
 */
void card__link_at_direction(struct card *card_1, struct card *card_2, enum direction direction);
 

/**
 * @brief Place the new card relatively to its neighbors and update their structure
 * @param new_card the card to place
 * @param neighbor_list the card neighbor list
 * @return 0 on success, -1 otherwise
 */
int card__place(struct card *new_card, struct card **neighbor_list);


/**
 * @brief draw a card
 * @param s the cards stack
 * @return the card dealt
 */
enum card_id card__draw(struct stack *s);


/**
 * @brief Sets the orientation of the card
 * @param card the card oriented
 * @param orientation the orientation wanted
 */
void card__set_orientation(struct card *card, enum orientation orientation);


#endif
