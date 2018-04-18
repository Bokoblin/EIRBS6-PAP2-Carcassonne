#include <assert.h>
#include "player.h"

struct player *player__init(unsigned int id, void *lib_ptr)
{
    struct player * p = safe_malloc(sizeof(struct player));

    p->id = id;
    p->score = 0;
    p->nb_meeples = DEFAULT_MEEPLE_NUMBER;
    p->lib_ptr = lib_ptr;
    p->get_player_name = NULL;
    p->initialize = NULL;
    p->play = NULL;
    p->finalize = NULL;

    return p;
}

void player__free(struct player *p)
{
    free(p);
}
