#ifndef DECK_H
#define DECK_H

#include "stack.h"

/**
 * @brief Creates a stack of enum card_id,
 *  according to the game's rules
 * @note Complexity O(n)
 * @return A struct stack of enum card_id
 */
struct stack* init_deck();

#endif
