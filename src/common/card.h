#ifndef CARDS_H
#define CARDS_H

#include "common_interface.h"
#include "ADT/stack.h"
#include "area_type.h"
#include "card_type.h"

#define DIRECTION_NUMBER 4
#define DEFAULT_ORIENTATION NORTH_TO_NORTH

////////////////////////////////////////////////////////////////////
///     STRUCTURES
////////////////////////////////////////////////////////////////////

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
  struct position pos;
};


////////////////////////////////////////////////////////////////////
///     COMMON FUNCTIONS
////////////////////////////////////////////////////////////////////

/**
 * @brief Initializes a card with the type card_type
 * @param card_type pointer towards the type of card wanted
 * @return a pointer towards a newly created card
 */
struct card* card__init(enum card_id card_id);


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
 * @brief Decide if two cards are matching in at least one direction
 * @param c1 a card
 * @param c2 a card
 * @return 1 if matching, 0 otherwise
 */
int card__are_matching(struct card *c1, struct card *c2);


/**
 * @brief decide if two cards match in a certain direction
 * @param c1 a card
 * @param c2 a card
 * @param direction the direction in which the cards are compared
 * @return 1 if the cards match, 0 otherwise
 */
int card__are_matching_direction(struct card *c1, struct card *c2, enum direction direction);


/**
 * @brief link two cards to each other in a certain direction
 * @param c1 a card
 * @param c2 a card
 * @param direction the direction in which the cards match (relative to card_1)
 */
void card__link_at_direction(struct card *c1, struct card *c2, enum direction direction);
 

/**
 * @brief Place the new card relatively to its neighbors and update their structure
 * @param new_card the card to place
 * @param pos the future position
 * @return 0 on success, -1 otherwise
 */
int card__place(struct card *new_card, struct position pos);


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
