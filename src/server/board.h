#ifndef BOARD_H
#define BOARD_H

#include "../common/common_interface.h"
#include "../common/card.h"


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
    struct set *cards_set;
    struct set *meeples_set;
};


////////////////////////////////////////////////////////////////////
///     FUNCTIONS
////////////////////////////////////////////////////////////////////


/** 
 * @brief Initialize a gameboard
 * @return a pointer towards a newly created board
 */
struct board *board__init(struct stack *drawing_stack);


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
