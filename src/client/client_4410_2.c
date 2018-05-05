#include "../common/common_interface.h"
#include "client.h"
#include <stdio.h>

struct client client;

/////////////////////////////////////////////////////////
/// Implementation of interface functions
/////////////////////////////////////////////////////////

char const* get_player_name()
{
    return("client4410-2");
}

void initialize(unsigned int id, unsigned int n_players)
{
    printf(CLI_PREF"Initializing client named: %s..."CLR"\n", get_player_name());
    client__init(&client, id, n_players);
}

struct move play(enum card_id card, struct move const previous_moves[], size_t n_moves)
{
    printf(CLI_PREF"Executing player turn of client named: %s..."CLR"\n", get_player_name());

    //Updating client own board
    client__update_board(&client, previous_moves, n_moves);

    //Choosing next move following board and drawn card
    return client__play_card(&client, card);
}

void finalize()
{
    printf(CLI_PREF"Finalizing client named: %s..."CLR"\n", get_player_name());
    client__free(&client);
}
