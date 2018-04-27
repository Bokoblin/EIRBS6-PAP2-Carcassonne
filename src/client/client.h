#ifndef CLIENT_H
#define CLIENT_H

#include "../server/board.h"

struct client{
    int id;
    int nb_players;
    int nb_meeples;
    struct board *client_board;
};

void client__init(struct client *client, unsigned int id, unsigned int n_players);

void client__free(struct client *client);

//TODO: Implement the functions underneath
int update_board(struct client *client, struct move const previous_moves[], size_t n_moves);

struct move play_card(struct client *client, enum card_id);

#endif
