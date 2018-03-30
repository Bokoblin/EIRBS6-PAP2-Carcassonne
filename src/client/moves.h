#ifndef __MOVES_H__
#define __MOVES_H__

#include "../common/projCarcassonneInterface.h"
#include "../common/cards.h"


/**
 * @brief Find an extremity on the gameboard 
 * @param board the gameboard
 * @return a struct 
 */
struct card *find_extremity(struct board* board);


/**
 * @brief Decide if a card can be placed next to another one
 * @param extremity is an extremity of the boardgame
 * @param placable_card is the card to try to place next to the extremity card
 * @return a boolean
 */
int is_valid_emplacement(struct card *extremity, struct card *placable_card);


#endif
