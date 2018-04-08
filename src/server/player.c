#include "player.h"

struct player* init_player()
{
    struct player * p = malloc(sizeof(struct player));

    if (p == NULL) {
        exit_on_error("Malloc failure on: struct client *");
    } else {
        p->lib_ptr = NULL;
        p->get_player_name = NULL;
        p->initialize = NULL;
        p->play = NULL;
        p->finalize = NULL;
    }

    return p;
}

void free_player(struct player *p)
{
    free(p);
}