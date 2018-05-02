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

int update_board(struct client *client, struct move const previous_moves[], size_t n_moves)
{
    assert_not_null(previous_moves, __func__, "moves parameter");

    int ret = 0;
    struct move move;
    struct card *c;
    for (size_t i = 0 ;i < n_moves; i++){
        move = previous_moves[i];
        if (move.check == VALID){
            c = card__init(move.card);
            c->pos = move.onto;
            card__set_orientation(c, (enum orientation) move.dir);
            if(board__add_card(client->client_board, c)){
                ret = 1;
                card__free(c);
            }
        }
    }
    return ret;
}


struct move play_card(struct client *client, enum card_id card)
{
    struct move played_move = { VALID, 0, card, { 3, 4 }, NORTH, NO_MEEPLE};
    (void) client;

    return played_move;
}
