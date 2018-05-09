#include "meeple.h"
#include "utils.h"

struct meeple *meeple__init(unsigned int player_id, struct card *card, enum place place)
{
    if (place != NO_MEEPLE) {
        struct meeple *m = safe_malloc(sizeof(struct meeple));
        m->player_id = player_id;
        m->related_card = card;
        m->position = place;
        m->belonging_area = card__get_relative_area(card, place);
        return m;
    } else {
        return NULL;
    }
}

void meeple__free(struct meeple *m)
{
    free(m);
}