#ifndef CARCASSONNE_INTERFACE_H
#define CARCASSONNE_INTERFACE_H

#include <stdlib.h>

////////////////////////////////////////////////////////////////////
///     PUBLIC STRUCTURES
////////////////////////////////////////////////////////////////////


/**
 * @brief Positions
 */
struct position { int x,y; };


/**
 * @brief Sides of the cards
 */
enum direction
{
	NORTH,
	WEST,
	SOUTH,
	EAST,
};


/**
 * @brief Places
 *
 * Lists all the possible places of the meeples on a card
 */
enum place
{
	NO_MEEPLE,      // special place for not placing a meeple
	POS_NORTH_EAST, // {NE}
	POS_NORTH,      // N
	POS_NORTH_WEST, // {NW}
	POS_WEST_NORTH, // {WN}
	POS_WEST,       // W
	POS_WEST_SOUTH, // {WS}
	POS_SOUTH_WEST, // {SW}
	POS_SOUTH,      // S
	POS_SOUTH_EAST, // {SE}
	POS_EAST_NORTH, // {EN}
	POS_EAST,       // E
	POS_EAST_SOUTH, // {ES}
	POS_CENTER,     // C
	LAST_POS,
};


/**
 * @brief Cards
 *
 * This list is the same as the one given on in the rules file :
 * http://riograndegames.com/getFile.php?id=670
 * There are 24 types of cards, listed here line by line, left to right.
 * All cards are described here by their directions, one per side (NWSE).
 * When a direction is not described, it is by default a plain.
 * NOTE: STARTING TILE: CARD_ROAD_STRAIGHT_CITY
 */
enum card_id
{
	CARD_MONASTERY_ROAD,      // monastery center, connected to one road S
	CARD_MONASTERY_ALONE,     // monastery center
	CARD_CITY_ALL_SIDES,      // connected cities on all sides
	CARD_ROAD_STRAIGHT_CITY,  // road connected N to S, city E
	CARD_CITY_ONE_SIDE,       // city N
	CARD_CITY_TUNNEL_SHLD,    // city connected E to W, shield
	CARD_CITY_TUNNEL,         // city connected N to S
	CARD_PLAIN_TUNNEL,        // plain connected N to S, city elsewhere
	CARD_PLAIN_TWO_CITIES,    // plain connected W to N, disconnected but jointive cities E and S
    CARD_ROAD_TURN_RIGHT_CITY,// city N, road connected S to E
	CARD_ROAD_TURN_LEFT_CITY, // city E, road connected W to N
	CARD_JUNCTION_CITY,       // city E, 3 roads elsewhere connected at center
	CARD_PLAIN_CITY_SHLD,     // city connected W to N, shield
	CARD_PLAIN_CITY,          // city connected W to N
	CARD_PLAIN_CITY_ROAD_SHLD,// city connected W to N, road connected S to E, shield
	CARD_PLAIN_CITY_ROAD,     // city connected W to N, road connected S to E
	CARD_CITY_THREE_SHLD,     // plain S, connected city elsewhere, shield
	CARD_CITY_THREE,          // plain S, connected city elsewhere
	CARD_CITY_THREE_ROAD_SHLD,// road S, connected city elsewhere, shield
	CARD_CITY_THREE_ROAD,     // road S, connected city elsewhere
	CARD_ROAD_STRAIGHT,       // road connected N to S
	CARD_ROAD_TURN,           // road connected W to S
	CARD_JUNCTION_THREE,      // plain N, 3 roads elsewhere connected at center
	CARD_JUNCTION_FOUR,       // 4 roads connected at the center
	LAST_CARD,
};


/**
 * @brief Authorized positions
 *
 * The allowed_positions table lists for each card the allowed
 * position of a meeple. Each list is terminated by NO_MEEPLE.
 */
static const enum place allowed_positions[LAST_CARD][LAST_POS] = {
		{ POS_NORTH, POS_SOUTH, POS_CENTER, NO_MEEPLE },
		{ POS_NORTH, POS_CENTER, NO_MEEPLE },
		{ POS_CENTER, NO_MEEPLE },
		{ POS_WEST, POS_EAST, POS_CENTER, POS_NORTH_EAST, NO_MEEPLE },
		{ POS_NORTH, POS_CENTER, NO_MEEPLE },
		{ POS_NORTH, POS_SOUTH, POS_CENTER, NO_MEEPLE },
		{ POS_WEST, POS_EAST, POS_CENTER, NO_MEEPLE },
		{ POS_WEST, POS_EAST, POS_CENTER, NO_MEEPLE },
		{ POS_SOUTH, POS_EAST, POS_CENTER, NO_MEEPLE },
		{ POS_NORTH, POS_WEST, POS_SOUTH_EAST, POS_SOUTH, NO_MEEPLE },
		{ POS_NORTH_WEST, POS_SOUTH, POS_EAST, POS_NORTH, NO_MEEPLE },
		{ POS_NORTH, POS_NORTH_WEST, POS_WEST, POS_SOUTH_WEST, POS_SOUTH,
				POS_EAST, POS_NORTH_EAST, NO_MEEPLE },
		{ POS_NORTH, POS_SOUTH, NO_MEEPLE },
		{ POS_NORTH, POS_SOUTH, NO_MEEPLE },
		{ POS_NORTH, POS_SOUTH, POS_SOUTH_EAST, POS_SOUTH_WEST, NO_MEEPLE },
		{ POS_NORTH, POS_SOUTH, POS_SOUTH_EAST, POS_SOUTH_WEST, NO_MEEPLE },
		{ POS_SOUTH, POS_CENTER, NO_MEEPLE },
		{ POS_SOUTH, POS_CENTER, NO_MEEPLE },
		{ POS_SOUTH, POS_SOUTH_EAST, POS_SOUTH_WEST, POS_CENTER, NO_MEEPLE },
		{ POS_SOUTH, POS_SOUTH_EAST, POS_SOUTH_WEST, POS_CENTER, NO_MEEPLE },
		{ POS_WEST, POS_EAST, POS_CENTER, NO_MEEPLE },
		{ POS_NORTH_EAST, POS_SOUTH_WEST, POS_WEST, NO_MEEPLE },
		{ POS_NORTH, POS_WEST, POS_SOUTH_WEST, POS_SOUTH, POS_SOUTH_EAST,
				POS_EAST, NO_MEEPLE },
		{ POS_NORTH, POS_NORTH_WEST, POS_WEST, POS_SOUTH_WEST, POS_SOUTH,
				POS_SOUTH_EAST, POS_EAST, POS_NORTH_EAST, NO_MEEPLE }
};


/**
 * @brief Action
 */
enum action
{
	VALID,             // valid action
	FAILED,            // invalid action
};


/**
 * @brief Moves
 */
struct move
{
	enum action check;     // validation of the move by the server
	unsigned int player;   // player issueing the move
	enum card_id card;     // card played
	struct position onto;  // position on the board
	enum direction dir;    // direction of the north of the card
	enum place place;      // place of the meeple on the card
};


////////////////////////////////////////////////////////////////////
///     PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////

/**
 * Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
char const* get_player_name();


/**
 * Player initialization
 * PARAM:
 * - id: player ID
 * - n_players: number of players in the game
 * PRECOND:
 * - 0 <= id < n_players
 */
void initialize(unsigned int id, unsigned int n_players);


/**
 * Computes next move
 * PARAM:
 * - card: the id of the card that has been drawn
 * - previous_moves: ordered list of previous moves starting from the last
 *   move issued by the player.
 * - n_moves: number of moves in previous_moves
 * PRECOND:
 * - card is playable in the current state of the game, i.e there is at least
 *   one location on the board where card can be played
 * - previous_moves is an array of at least n_moves elements.
 * - previous_moves is an ordered list of previous moves starting from the last
 *   move of the player. Every move invalidated by the server has action
 *   FAILED. Every move validated by the server is faithfully transcribed.
 * RETURNS:
 * - the next move for the player.
 */
struct move play(enum card_id card, struct move const previous_moves[], size_t n_moves);


/**
 * Announces the end of the game to the player, and cleans up the
 * memory he may have been using.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize();


#endif // CARCASSONNE_INTERFACE_H
