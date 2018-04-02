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
    struct card* first_card;
    unsigned int fp_capacity;
    struct card** free_positions;
};


////////////////////////////////////////////////////////////////////
///     FUNCTIONS
////////////////////////////////////////////////////////////////////


/** 
 * @brief Initialize a gameboard
 * @return a pointer towards a newly created board
 */
struct board* init_board();


/**
 * @brief Release memory allocated to the gameboard
 * @param game the gameboard
 */
void free_board(struct board* game);

#endif

//TODO: Arthur : Tests
