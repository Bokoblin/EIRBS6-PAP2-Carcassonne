#ifndef SERVER_H
#define SERVER_H

#include "../common/ADT/queue.h"
#include "../common/ADT/stack.h"
#include "../common/card.h"
#include "../common/com_func_ptrs.h"
#include "../common/deck.h"
#include "../common/utils.h"
#include "../server/board.h"
#include "../server/player.h"
#include "server_func_ptrs.h"

#define DEFAULT_GRAPHIC_MODE_FLAG 0
#define DEFAULT_CLIENT_COUNT 0
#define NOT_APPLICABLE 2


/**
 * @brief Register all player libraries passed in program arguments
 * @param argc argument count
 * @param argv argument content array
 * @param players the players container
 * @param nb_players the number of players to register
 */
void register_players(int argc, const char **argv, struct queue *players, unsigned int nb_players);


/**
 * @brief Verify that a player move is valid
 * @param b the board
 * @param p the player
 * @param m the move
 * @return true if verification is a success, false otherwise
 */
int is_valid_play(struct board *b, struct player *p, struct move *m);


/**
 * @brief Draw a card until it is valid
 * @param b the board
 * @param s the card stack
 * @return a valid card id
 */
enum card_id draw_until_valid(struct board* b, struct stack *s);


/**
 * @brief Convert a move queue into an array for communication with the client
 * @param moves the moves queue
 * @return a move array
 */
struct move *build_previous_moves_array(struct queue *moves);


/**
 * @brief Free resources used by player structure
 * @param p a player
 */
void free_player_resources(struct player *p);

/**
 * @brief Free all resources used
 * @param players_queue the player list in a queue
 */
void free_resources(struct queue *players_queue);


/**
 * @brief Contain the game main (game initialization, game loop and game finalization)
 * @param players the players queue
 * @param nb_player the number of players
 */
void game_main(struct queue *players, unsigned int nb_player);


/**
 * @brief Init the first player of the queue
 * @param players_queue the player queue
 * @param nb_player the players number
 */
void init_next_player(struct queue *players_queue, unsigned int nb_player);


/**
 * @brief Finalize the first player of the queue
 * @param players_queue the player queue
 */
void finalize_next_player(struct queue *players_queue);

#endif
