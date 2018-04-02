#ifndef MOVES_H
#define MOVES_H

#include "../common/common_interface.h"
#include "../common/card.h"


/**
 * @brief Find an extremity on the gameboard 
 * @param board the gameboard
 * @return a struct 
 */
struct card *find_extremity(struct board* board);


/**
 * @brief Decide if a card can be placed next to another one
 * @param extremity is an extremity of the gameboard
 * @param placeable_card is the card to try to place next to the extremity card
 * @return a boolean
 */
int is_valid_emplacement(struct card *extremity, struct card *placeable_card);


#endif
