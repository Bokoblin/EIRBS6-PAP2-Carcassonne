#include <stdlib.h>
#include "card.h"
#include "utils.h"

struct card* card__empty(struct card_type card_type)
{
    struct card * c = malloc(sizeof(struct card));
    if (c == NULL) {
        exit_on_error("Malloc failure on: struct card*");
    } else {
        c->type = card_type;
        c->orientation = DEFAULT_ORIENTATION;
    }

    return c;
}

void card__free(struct card *card)
{
    free(card);
}

enum area_type card__get_area(struct card *card, enum place place)
{
    if (place == POS_CENTER) return card->type.areas[place-1];
    return card->type.areas[(place-1+3*card->orientation)%12];
}

int card__are_matching_direction(struct card *card_1, struct card *card_2, enum direction direction)
{
    for(int i = 0 ; i < 3 ; i++){
        enum place p = (direction * 3 + 1 + i); //FIXME: place can't be casted to direction
        if(card__get_area(card_1, p) != card__get_area(card_2, p))
            return 0;
    }
    return 1;
}

void card__link_at_direction(struct card *card_1, struct card *card_2, enum direction direction)
{
    card_1->neighbors[direction] = card_2; //FIXME: is it neighbors ? I wasn't sure
    card_2->neighbors[(direction+2)%4] = card_1;
}

int card__place(struct card *new_card, struct card **neighbor_list)
{
    //TODO : card__place impl

    return -1;
}

enum card_id card__draw(struct stack *s)
{
    enum card_id *poped_card = stack__pop(s);
    enum card_id returned = *poped_card;
    free(poped_card);
    return returned;
}

void card__set_orientation(struct card *card, enum orientation orientation)
{
    card->orientation = orientation;
}
