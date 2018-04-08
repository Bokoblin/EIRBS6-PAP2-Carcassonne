#ifndef BOARD_H
#define BOARD_H

#include "common_interface.h"
#include "card.h"


////////////////////////////////////////////////////////////////////
///     STRUCTURE
////////////////////////////////////////////////////////////////////


/**
 * @brief Struct board
 * Points out the first card
 */
struct board
{
    struct card** free_positions_array;
    unsigned int fp_capacity;
    unsigned int fp_size;
};


////////////////////////////////////////////////////////////////////
///     FUNCTIONS
////////////////////////////////////////////////////////////////////


/** 
 * @brief Initialize a gameboard
 * @return a pointer towards a newly created board
 */
struct board* board__empty();


/**
 * @brief Indicate first card of the board
 * @param b the gameboard
 * @return a card pointer
 */
struct card* board__first_card(struct board *b);


/**
 * @brief Add a card to the board
 * @param b the board
 * @param c a card
 * @return 0 on succes, -1 otherwise
 */
int board__add_card(struct board *b, struct card* c);


/**
 * @brief Release memory allocated to the gameboard
 * @param b the gameboard
 */
void board__free(struct board *b);

#endif
