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
    client->client_board = board__init();
}

void client__free(struct client *client)
{
  board__free(client->client_board);
}

int update_board(struct client *client, struct move const previous_moves[], size_t n_moves){
    int ret = 0;
    for (size_t i = 0 ;i < n_moves; i++){
        struct move const move = previous_moves[i];
        if (move.check == VALID){
            struct card *c = card__init(move.card);
            c->pos = move.onto;
            card__set_orientation(c, (enum orientation) move.dir);
            if(board__add_card(client->client_board, c)) ret = 1;
        }
    }
    return ret;
}

