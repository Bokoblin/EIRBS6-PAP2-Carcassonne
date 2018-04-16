#include <assert.h>
#include "player.h"

struct player *player__init(unsigned int id, void *lib_ptr, void *get_name_ptr,
                            void *init_ptr, void *play_ptr, void *final_ptr)
{
    struct player * p = safe_malloc(sizeof(struct player));

    p->id = id;
    p->score = 0;
    p->nb_meeples = DEFAULT_MEEPLE_NUMBER;
    p->lib_ptr = lib_ptr;
    p->get_player_name = get_name_ptr;
    p->initialize = init_ptr;
    p->play = play_ptr;
    p->finalize = final_ptr;

    return p;
}

void player__free(struct player *p)
{
    free(p);
}
