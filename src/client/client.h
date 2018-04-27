#ifndef CLIENT_H
#define CLIENT_H
#include "../common/meeple.h"
#include "../server/board.h"

struct client{
    int id;
    int nb_meeples;
    struct meeple meeples[MAX_MEEPLES];
    char *name;
    struct board client_board;
};

struct client client;

int ask_for_player_name(struct client *client);

#endif
