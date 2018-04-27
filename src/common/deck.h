#ifndef DECK_H
#define DECK_H

#include "ADT/stack.h"

#define CARDS_NUMBER 72

/**
 * @brief Fill a stack of enum card_id
 * according to the game's rules
 * @note Complexity O(number of card)
 * @param s a stack to fill
 */
void init_deck(struct stack* s);

#endif
