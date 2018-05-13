#ifndef GAME_H
#define GAME_H

#include "../common/ADT/queue.h"
#include "../common/ADT/stack.h"
#include "../common/card.h"
#include "../common/deck.h"
#include "../common/utils.h"
#include "../server/board.h"
#include "../server/player.h"

#define NOT_APPLICABLE 2

////////////////////////////////////////////////////////////////////
///     STRUCTURE
////////////////////////////////////////////////////////////////////

struct game
{
    unsigned int nb_players;
    struct queue *players_queue;
};


////////////////////////////////////////////////////////////////////
///     FUNCTIONS
////////////////////////////////////////////////////////////////////


/**
 * @brief Init the game
 * @param argc the program arguments count
 * @param argv the program arguments
 * @param nb_players the number of player to init
 * @return a newly created game
 */
struct game *game__init(int argc, char **argv, unsigned int nb_players);


/**
 * @brief Free the game resources
 * @param g the game
 */
void game__free(struct game *g);


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
 * @param game the game core
 */
void game__main(struct game *game);


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
