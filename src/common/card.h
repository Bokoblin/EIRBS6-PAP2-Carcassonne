#ifndef CARDS_H
#define CARDS_H

#include "common_interface.h"
#include "ADT/stack.h"
#include "area_type.h"
#include "card_type.h"

#define DIRECTION_NUMBER 4
#define SIDES_NUMBER 4
#define SIDE_SUBAREAS_NUMBER 3
#define DEFAULT_DIRECTION NORTH
#define INVALID_POSITION 999

////////////////////////////////////////////////////////////////////
///     STRUCTURES
////////////////////////////////////////////////////////////////////

/**
 * @brief card_side
 * Define the sides of the card
 */
enum card_side
{
    NORTH_SIDE,
    WEST_SIDE,
    SOUTH_SIDE,
    EAST_SIDE
};

/**
 * @brief card structure
 */
struct card
{
    struct card_type type;
    struct card * neighbors[SIDES_NUMBER]; // Indexed by enum card_side
    enum direction direction;
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
 * @brief Get the position of a potential card at the given side of the given card
 * @param card a card
 * @param concerned_side a side
 * @return a position
 */
struct position card__get_position_at_side(struct card *card, enum card_side concerned_side);


/**
 * @brief Decide if two cards are matching in at least one direction
 * @param c1 a card
 * @param c2 a card
 * @return 1 if matching, 0 otherwise
 */
int card__are_matching_free_side(struct card *c1, struct card *c2);


/**
 * @brief Decide if two cards following given sides
 * @param c1 a card
 * @param c2 a card
 * @param s1 the side in which the first card coincide with the second
 * @param s2 the side in which the second card coincide with the first
 * @return 1 if the cards match, 0 otherwise
 */
int card__are_matching_sides(struct card *c1, struct card *c2, enum card_side s1, enum card_side s2);

/**
 * @brief link two cards to each other at specific sides
 * @param c1 a card
 * @param c2 a card
 * @param s1 the sides in which the card 1 matches the card 2
 * @param s2 the sides in which the card 2 matches the card 1
 * @return 1 if the cards link, 0 otherwise
 */
int card__link_at_sides(struct card *c1, struct card *c2, enum card_side s1, enum card_side s2);


/**
 * @brief draw a card
 * @param s the cards stack
 * @return the card dealt
 */
enum card_id card__draw(struct stack *s);


#endif
