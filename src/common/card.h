#ifndef CARDS_H
#define CARDS_H

#include "common_interface.h"
#include "ADT/stack.h"
#include "area_type.h"
#include "card_type.h"

#define DIRECTION_NUMBER 4
#define DEFAULT_ORIENTATION NORTH_IS_NORTH_SIDE
#define INVALID_POSITION 999

////////////////////////////////////////////////////////////////////
///     STRUCTURES
////////////////////////////////////////////////////////////////////

/**
 * @brief orientation
 * Lists all the cards orientations indicating where is the north
 */
enum orientation
{
    NORTH_IS_NORTH_SIDE, // the north of card_type coincide with the north of the card
    NORTH_IS_WEST_SIDE,  // the north of card_type coincide with the west of the card
    NORTH_IS_SOUTH_SIDE, // the north of card_type coincide with the south of the card
    NORTH_IS_EAST_SIDE   // the north of card_type coincide with the east of the card
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
///     FUNCTIONS
////////////////////////////////////////////////////////////////////

/**
 * @brief Initialize a card with the type card_type
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
 * @brief Get the area type
 * @param card a card
 * @param place the zone that we want to get
 * @return the area_type of the area place
 */
enum area_type card__get_area(struct card *card, enum place place);


/**
 * @brief Get the number of neighbours a card has
 * @param card a card
 * @return
 */
unsigned int card__get_neighbour_number(struct card *card);


/**
 * @brief Get the position at the given direction of the given card
 * @param card a card
 * @param direction a direction
 * @return a position
 */
struct position card__get_position_at_direction(struct card *card, enum direction direction);


/**
 * @brief Decide if two cards are matching in at least one direction
 * @param c1 a card
 * @param c2 a card
 * @return 1 if matching, 0 otherwise
 */
int card__are_matching_free_side(struct card *c1, struct card *c2);


/**
 * !!!!!!!!!!!!!! DEPRECATED !!!!!!!!!!!!!!
 * @brief Decide if two cards are matching at first card direction
 * @param c1 a card
 * @param c2 a card
 * @param direction the direction in which the first card coincide with the second
 * @return 1 if the cards match, 0 otherwise
 */
//int card__are_matching_direction(struct card *c1, struct card *c2, enum direction direction);


/**
 * @brief Decide if two cards following given directions
 * @param c1 a card
 * @param c2 a card
 * @param d1 the direction in which the first card coincide with the second
 * @param d2 the direction in which the second card coincide with the first
 * @return 1 if the cards match, 0 otherwise
 */
int card__are_matching_directions(struct card *c1, struct card *c2, enum direction d1, enum direction d2);


/**
 * !!!!!!!!!!!!!! DEPRECATED !!!!!!!!!!!!!!
 * @brief link two cards to each other in a certain direction
 * @param c1 a card
 * @param c2 a card
 * @param direction the direction in which the cards match (relative to card_1)
 * @return 0 if the cards link, 1 otherwise
 */
//int card__link_at_direction(struct card *c1, struct card *c2, enum direction direction);

/**
 * @brief link two cards to each other at specific directions
 * @param c1 a card
 * @param c2 a card
 * @param d1 the direction in which the card 1 matches the card 2
 * @param d2 the direction in which the card 2 matches the card 1
 * @return 0 if the cards link, 1 otherwise
 */
int card__link_at_directions(struct card *c1, struct card *c2, enum direction d1, enum direction d2);


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
