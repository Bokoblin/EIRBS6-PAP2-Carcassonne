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
    printf("\x1B[35m[CLIENT] Initializing client named: %s...\x1B[0m\n", get_player_name());
    client__init(&client, id, n_players);
}

struct move play(enum card_id card, struct move const previous_moves[], size_t n_moves)
{
    printf("\x1B[35m[CLIENT] Executing player turn of client named: %s...\x1B[0m\n", get_player_name());

    //Updating player number
    client.nb_players = (int) n_moves;

    //Updating client own board
    //TODO : update board

    //Choosing next move following board and drawn card
    //TODO: choose next move

    (void) card;
    (void) previous_moves;
    struct move move_placeholder = { VALID, 0, card, { 3, 4 }, NORTH, NO_MEEPLE};
    return move_placeholder;
}

void finalize()
{
    printf("\x1B[35m[CLIENT] Finalizing client named: %s...\x1B[0m\n", get_player_name());
    client__free(&client);
}
