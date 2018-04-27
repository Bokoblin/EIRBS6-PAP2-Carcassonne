#include "../common/common_interface.h"
#include "client.h"
#include <stdio.h>

struct client client;

/////////////////////////////////////////////////////////
/// Implementation of interface functions
/////////////////////////////////////////////////////////

char const* get_player_name()
{
    return("client4410-1");
}

void initialize(unsigned int id, unsigned int n_players)
{
    client__init(&client, id, n_players);
    printf("\x1B[35m[CLIENT] Executing placeholder %s::initialize()...\x1B[0m\n", get_player_name());
}

struct move play(enum card_id card, struct move const previous_moves[], size_t n_moves)
{
    (void) card;
    (void) previous_moves;
    (void) n_moves;

    client.nb_players = n_moves;

    //TODO: Implement client "play"
    printf("\x1B[35m[CLIENT] Executing placeholder %s::play()...\x1B[0m\n", get_player_name());

    struct move move_placeholder = { VALID, 0, card, { 3, 4 }, NORTH, NO_MEEPLE};
    return move_placeholder;
}

void finalize()
{
    client__free(&client);
    printf("\x1B[35m[CLIENT] Executing placeholder %s::finalize()...\x1B[0m\n", get_player_name());
}
