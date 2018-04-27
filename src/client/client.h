#ifndef CLIENT_H
#define CLIENT_H
#include "../common/meeple.h"
#include "../server/board.h"

struct client{
    int id;
    int nb_players;
    int nb_meeples;
    struct meeple meeples[MAX_MEEPLES];
    struct board client_board;
};

#endif
