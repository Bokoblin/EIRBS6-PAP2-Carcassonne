#ifndef __PILE_H__
#define __PILE_H__

struct pile;


/**
 * @brief Decide if a card can be placed next to another one
 * @param extremity is an extremity of the boardgame
 * @param placable_card is the card to try to place next to the extremity card
 * @return a boolean
 */
struct pile *pile__empty();


/**
 * @brief Stack an element in the pile
 * @param pile is to pile you want to stack on
 * @param element stacked in the pile
 * @return 0 if stacked successfully, -1 if an error occured
 */
int pile__stack(struct pile *pile, void *element);


/**
 * @brief Unstack an element from the pile
 * @param pile is to pile you want to unstack from
 * @return a pointor to the unstacked element
 */
void *pile__unstack(struct pile *pile);


/**
 * @brief Free all allocated memory used by the pile
 * @param pile is to pile you want to free
 * @return 0 if stacked successfully, -1 if an error occured
 */
int pile__free(struct pile *pile);


#endif
