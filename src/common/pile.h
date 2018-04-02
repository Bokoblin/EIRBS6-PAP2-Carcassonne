#ifndef __PILE_H__
#define __PILE_H__

#include "projCarcassonneInterface.h"

struct pile;

/**
 * @brief Create an empty pile (O(1))
 * @param user_free is a function that free allocated memory used by the type you want to stock
 * @param user_copy is a function that create a copy of the type you want to stock, and returns a pointor to this copy
 * @return user_debug is a function that prints the type you want to stock
 */
struct pile *pile__empty();


/**
 * @brief Verify if the pile is empty (O(1))
 * @param pile is to pile you want to check on
 * @return a boolean
 */
int pile__is_empty(struct pile *pile);

  
/**
 * @brief Stack an element in the pile (O(1))
 * @param pile is to pile you want to stack on
 * @param element stacked in the pile
 * @return 0 if stacked successfully, -1 if an error occured
 */
int pile__stack(struct pile *pile, enum card_id element);


/**
 * @brief Unstack an element from the pile (O(1))
 * @param pile is to pile you want to unstack from
 * @return a pointor to the unstacked element
 */
enum card_id pile__unstack(struct pile *pile);


/**
 * @brief Count how many elements are in the pile (O(1))
 * @param pile is to pile you want to know the length
 * @return an integer corresponding to the number of elements in the pile
 */
unsigned int pile__length(struct pile *pile);


/**
 * @brief Free all allocated memory used by the pile (O(n))
 * @param pile is to pile you want to free
 */
void pile__free(struct pile *pile);


/**
 * @brief Prints the pile's content (O(n))
 * @param pile is to pile you want to print
 */
void pile__debug(struct pile *pile);
  
#endif
