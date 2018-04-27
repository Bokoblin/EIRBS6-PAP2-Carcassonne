#include "../common/common_interface.h"
#include <stdio.h>

/////////////////////////////////////////////////////////
/// Structures
/////////////////////////////////////////////////////////

struct client{
    int id;
    int nb_meeples;
    struct meeple meeples[MAX_MEEPLES];
    char *name;
    struct board client_board;
};

struct client client;

/////////////////////////////////////////////////////////
/// Implementation of interface functions
/////////////////////////////////////////////////////////

char const* get_player_name()
{
    return client.name;
}

void initialize(unsigned int id, unsigned int n_players)
{

    client.id = id;
    (void) n_players;
    //TODO: Implement client "initialize"
    printf("\x1B[35m[CLIENT] Executing placeholder %s::initialize()...\x1B[0m\n", get_player_name());
}

struct move play(enum card_id card, struct move const previous_moves[], size_t n_moves)
{
    (void) card;
    (void) previous_moves;
    (void) n_moves;

    //TODO: Implement client "play"
    printf("\x1B[35m[CLIENT] Executing placeholder %s::play()...\x1B[0m\n", get_player_name());

    struct move move_placeholder = { VALID, 0, card, { 3, 4 }, NORTH, NO_MEEPLE};
    return move_placeholder;
}

void finalize()
{
    //TODO: Implement client "finalize"
    printf("\x1B[35m[CLIENT] Executing placeholder %s::finalize()...\x1B[0m\n", get_player_name());
}
