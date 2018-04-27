#ifndef CLIENT_H
#define CLIENT_H

#include "../server/board.h"

/////////////////////////////////////////////////////////////
/// STRUCTURE
/////////////////////////////////////////////////////////////

struct client{
    int id;
    int nb_players;
    int nb_meeples;
    struct board *client_board;
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
 * @return 0 if done successfully, 1 otherwise
 */
int update_board(struct client *client, struct move const previous_moves[], size_t n_moves);


//TODO: Implement the functions underneath
/**
* @brief search for a playable position for a card on the client's gameboard
* @param client the client
* @param card_id the id of the card to play
* @return struct_move
*/
struct move play_card(struct client *client, enum card_id);

#endif
