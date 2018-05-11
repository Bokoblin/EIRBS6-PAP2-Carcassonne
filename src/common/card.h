#ifndef CARDS_H
#define CARDS_H

#include "common_interface.h"
#include "card_type.h"

#define NB_DIRECTIONS 4
#define NB_SUB_DIRECTIONS 3      //divisions in a direction e.g. for north : NE, N, NW
#define DEFAULT_DIRECTION NORTH
#define INVALID_POSITION 999
#define PLACE_SHIFT 1               //because interface as placed NO_MEEPLE in pos 0


/**
 * @brief card structure
 */
struct card
{
    struct card_type type;
    struct card * neighbors[NB_DIRECTIONS]; //order regardless of direction
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
 * @brief Get the area type at place relatively to the card
 * @example if card direction is west and place NW, we thus need relative NW which is  WS
 * @param card a card
 * @param place the zone that we want to get
 * @return the area_type of the area place
 */
enum area_type card__get_relative_area(struct card *card, enum place place);


/**
 * @warning !!! ALPHA FUNCTION !!!
 * @brief Get the area type at place relatively of the environment
 * @example if card direction is west and place NW, we thus need absolute/real NW which is EN
 * @param card a card
 * @param place the zone that we want to get
 * @return the area_type of the area place
 */
enum area_type card__get_absolute_area(struct card *card, enum place place);


/**
 * @brief Get the number of neighbours a card has
 * @param card a card
 * @return
 */
unsigned int card__get_neighbour_number(struct card *card);


/**
 * @brief Get the position of a potential card at the given direction regardless of card direction
 * @param card a card
 * @param dir a direction
 * @return a position
 */
struct position card__get_position_at_direction(struct card *card, enum direction dir);


/**
 * @brief Decide if two cards are matching in at least one direction
 * @param c1 a card
 * @param c2 a card
 * @return 1 if matching, 0 otherwise
 */
int card__are_matching_free_direction(struct card *c1, struct card *c2);


/**
 * @brief Decide if two cards following given sides
 * @param c1 a card
 * @param c2 a card
 * @param d1 the side in which the first card coincide with the second
 * @param d2 the side in which the second card coincide with the first
 * @return 1 if the cards match, 0 otherwise
 */
int card__are_matching_directions(struct card *c1, struct card *c2, enum direction d1, enum direction d2);


/**
 * @brief link two cards to each other at specific sides
 * @param c1 a card
 * @param c2 a card
 * @param d1 the sides in which the card 1 matches the card 2
 * @param d2 the sides in which the card 2 matches the card 1
 * @return 1 if the cards link, 0 otherwise
 */
int card__link_at_directions(struct card *c1, struct card *c2, enum direction d1, enum direction d2);


/**
 * @brief Unlink all neighbours of a card
 * @param c the card
 */
void card__unlink_neighbours(struct card* c);

#endif
