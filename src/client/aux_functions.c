#include "../common/common_interface.h"
#include "client.h"
#include "../common/utils.h"
#include "../server/board.h"
#include "../common/meeple.h"

/////////////////////////////////////////////////////////
/// Implementation of client useful functions
/////////////////////////////////////////////////////////

void client__init(struct client *client, unsigned int id, unsigned int n_players)
{
    client->id = id;
    client->nb_players = n_players;
    client->nb_meeples = MAX_MEEPLES;
    client->client_board = board__init(NULL);
}

void client__free(struct client *client)
{
  board__free(client->client_board);
}