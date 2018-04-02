#ifndef CARDS_H
#define CARDS_H

#include "common_interface.h"
#include "stack.h"
#include "area_type.h"


////////////////////////////////////////////////////////////////////
///     STRUCTURE
////////////////////////////////////////////////////////////////////


/**
 * @brief Struct card
 * Describes a single card
 */
struct card
{
    enum card_id id;
    struct card* north;
    struct card* south;
    struct card* east;
    struct card* west;
    enum area_type type_north_east; // {NE}
    enum area_type type_north; // {N}
    enum area_type type_north_west; // {NW}
    enum area_type type_west_north; // {WN}
    enum area_type type_west; // {W}
    enum area_type type_west_south; // {WS}
    enum area_type type_south_west; // {SW}
    enum area_type type_south; // {S}
    enum area_type type_south_east; // {SE}
    enum area_type type_east_north; // {EN}
    enum area_type type_east; // {E}
    enum area_type type_east_south; // {ES}
    enum area_type type_center; // {C}
};


////////////////////////////////////////////////////////////////////
///     COMMON FUNCTIONS
////////////////////////////////////////////////////////////////////

/** 
 * @brief Initialize a card depending on its identity
 * @param id card ID 
 * @return a pointer towards a newly created card
 */
struct card* card__empty(enum card_id id);

/**
 * @brief Release memory allocated to the card structure
 * @param game the gameboard
 */
void card__free(struct card *card);


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
 * @brief Rotate the given card in the clockwise turn
 * @param card the card to rotate
 */
void card__clockwise_rotation(struct card *card);


/**
 * @brief Rotate the given card in the trigonometric turn
 * @param card the card to rotate
 */
void card__trigo_rotation(struct card *card);

#endif

//TODO: Arthur : Tests
