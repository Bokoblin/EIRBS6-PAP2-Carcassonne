#ifndef CLIENT_H
#define CLIENT_H

#include "../../common/interface.h"
#include "../../common/meeple.h"
#include "../../common/utils.h"
#include "micro_board.h"

/////////////////////////////////////////////////////////////
/// STRUCTURE
/////////////////////////////////////////////////////////////

struct client
{
    unsigned int id;
    unsigned int nb_players;
    unsigned int nb_meeples;
    struct micro_board *board;
};

/////////////////////////////////////////////////////////////
/// FUNCTIONS
/////////////////////////////////////////////////////////////

/**
 * @brief initialize a client
 * @param client the client who should be initialized
 * @param id the client's id
 * @param n_players the number of players
 */
void client__init(struct client *client, unsigned int id, unsigned int n_players);


/**
 * @brief Release memory allocated to client
 * @param client the client
 */
void client__free(struct client *client);


/**
 * @brief Update the gameboard of client
 * @param client the client who wants to change his gameboard
 * @param previous_moves array of the moves that should be added to the gameboard
 * @param n_moves size of previous_moves
 */
void client__update_board(struct client *client, struct move const previous_moves[], size_t n_moves);


/**
 * @brief Init a move
 * @param player_id the player id
 * @param ci the drawn card id
 * @param onto the position
 * @param north_dir the north dir of the card
 * @return a move
 */
struct move move__init(unsigned int player_id, enum card_id ci, struct position onto, enum direction north_dir);


/**
 * @brief Populate the list of possible moves
 * @param c the client
 * @param possible_moves the list
 * @param ci the card id drawn by the server
 */
void client__populate_possible_moves_list(struct client *c, struct set *possible_moves, enum card_id ci);


/**
 * @brief Handle how the client chooses a move among all possibles
 * @note This function must be implemented foreach client
 * @param possible_moves the list of possible moves
 * @return the chosen move
 */
struct move client__chose_move_strategy(struct set *possible_moves);


/**
 * @brief Display debug information on client
 * @param client the client
 */
void client__debug(struct client *client);

#endif
