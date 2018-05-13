#ifndef MICRO_BOARD_H
#define MICRO_BOARD_H

#include "../../common/interface.h"
#include "../../common/card.h"
#include "../../common/ADT/set.h"
#include "../../common/ADT/queue.h"


////////////////////////////////////////////////////////////////////
///     STRUCTURE
////////////////////////////////////////////////////////////////////


/**
 * @brief Struct micro board
 * @details a micro version of the server board
 */
struct micro_board
{
    struct card* first_card;
    struct set* cards_set;
};


////////////////////////////////////////////////////////////////////
///     FUNCTIONS
////////////////////////////////////////////////////////////////////


/** 
 * @brief Initialize a gameboard
 * @return a pointer towards a newly created board
 */
struct micro_board *micro_board__init();


/**
 * @brief Retrieves a card in a set following its position.
 * @warning The set must have been initialized with a compare operator based on the position
 * @param set the set
 * @param pos the position of the searched card
 * @return the card or NULL if not found
 */
struct card* micro_board__retrieve_card_by_position(struct set *set, struct position pos);


/**
 * @brief Checks if card adding is possible
 * @param b the board
 * @param card a card
 * @return  on success, 1 otherwise
 */
int micro_board__is_card_adding_possible(struct micro_board *b, struct card *card);


/**
 * @brief Add a card to the board without any check
 * @note Above function must be used first
 * @param b the board
 * @param card the card
 * @return 0 on success, 1 otherwise
 */
int micro_board__add_card_to_board(struct micro_board *b, struct card *card);


/**
 * @brief Release memory allocated to the gameboard
 * @param b the gameboard
 */
void micro_board__free(struct micro_board *b);

#endif
