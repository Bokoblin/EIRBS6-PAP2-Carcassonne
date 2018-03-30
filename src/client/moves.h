#ifndef __MOVES_H__
#define __MOVES_H__

#include "../common/projCarcassonneInterface.h"
#include "../common/cards.h"


/**
 * @brief Find an extremity on the boardgame from the start point card
 */
struct card *find_extremity(struct card *start_point);


/**
 * @brief Decide if a card can be placed next to another one
 * @param extremity is an extremity of the boardgame
 * @param placable_card is the card to try to place next to the extremity card
 * @return a boolean
 */
int is_valid_emplacement(struct card *extremity, struct card *placable_card);


/**
 * @brief Rotate the given card in the clockwise turn
 * @param card is the card you want to rotate
 */
void clokwise_rotation(struct card *card);


/**
 * @brief Rotate the given card in the trigonometric turn
 * @param card is the card you want to rotate
 */
void trigo_rotation(struct card *card);

#endif
