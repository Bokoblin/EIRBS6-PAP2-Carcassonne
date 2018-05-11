#ifndef BOARD_H
#define BOARD_H

#include "../common/common_interface.h"
#include "../common/card.h"
#include "../common/meeple.h"
#include "../common/ADT/set.h"
#include "../common/ADT/queue.h"


////////////////////////////////////////////////////////////////////
///     STRUCTURE
////////////////////////////////////////////////////////////////////


/**
 * @brief Struct board
 * Points out the first card
 */
struct board
{
    struct stack* drawing_stack;
    struct card* first_card;
    struct set* cards_set;
    struct set* meeples_set;
    struct queue* moves_queue;
};


////////////////////////////////////////////////////////////////////
///     FUNCTIONS
////////////////////////////////////////////////////////////////////


/** 
 * @brief Initialize a gameboard
 * @return a pointer towards a newly created board
 */
struct board *board__init();


/**
 * @brief Init the first card from drawing stack top card
 * @param b the board
 * @return 0 on success, 1 otherwise
 */
int board__init_first_card(struct board *b);


/**
 * @brief Verify that a card can be played in next turn
 * @details Verifies that a card matches with another on the board
 * @param b the board
 * @param ci the card id to verify
 * @return true if verification is a success, false otherwise
 */
int board__is_valid_card(struct board *b, enum card_id ci);


/**
 * @brief Retrieves a card in a set following its position.
 * @warning The set must have been initialized with a compare operator based on the position
 * @param set the set
 * @param pos the position of the searched card
 * @return the card or NULL if not found
 */
struct card* board__retrieve_card_by_position(struct set *set, struct position pos);


/**
 * @brief Add a card to the board (if possible)
 * @param b the board
 * @param card_to_add a card
 * @return 0 on success, 1 otherwise
 */
int board__add_card(struct board *b, struct card* card_to_add);


/**
 * @brief Add a meeple to the board on a card (if possible)
 * @param b the board
 * @param m the meeple to place
 * @return 0 on success, 1 otherwise
 */
int board__add_meeple(struct board *b, struct meeple *m);


/**
 * Checks if previous moves have permitted a board sub-completion and process it
 * (count score for area closed depending on meeples)
 * @param b the board
 */
void board__check_sub_completion(struct board *b);


/**
 * @brief Release memory allocated to the gameboard
 * @param b the gameboard
 */
void board__free(struct board *b);

#endif
