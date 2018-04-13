#include <stdlib.h>
#include "card.h"
#include "utils.h"


struct card* card__init(enum card_id card_id)
{
    struct card * c = malloc(sizeof(struct card));
    if (c == NULL) {
        exit_on_error("Malloc failure on: struct card*");
    } else {
        c->type = card__id_to_type(card_id);
        c->orientation = DEFAULT_ORIENTATION;
        c->pos.x = 999; //meaning invalid
        c->pos.y = 999;
    }

    return c;
}

void card__free(struct card *card)
{
    free(card);
}

enum area_type card__get_area(struct card *card, enum place place)
{
    if (card == NULL)
        exit_on_error("NULL value on card*"); //TODO : replace by assertNotNull(ptr, message)

    if (place == POS_CENTER)
        return card->type.areas[place-1];

    return card->type.areas[(place-1+3*card->orientation)%12];
}

int card__are_matching_direction(struct card *card_1, struct card *card_2, enum direction direction)
{
    if (card_1 == NULL || card_2 == NULL)
        exit_on_error("NULL value on card*");

    for(int i = 0 ; i < 3 ; i++) {
        unsigned int place_index = (direction * 3 + 1 + i);
        if (place_index > LAST_POS)
            exit_on_error("Out of range place");

        enum place p = (enum place) place_index;
        if(card__get_area(card_1, p) != card__get_area(card_2, p))
            return 0; //matching failure
    }

    return 1;
}

void card__link_at_direction(struct card *card_1, struct card *card_2, enum direction direction)
{
    if (card_1 == NULL || card_2 == NULL)
        exit_on_error("NULL value on card*");

    if (direction > DIRECTION_NUMBER)
        exit_on_error("Out of range direction");

    card_1->neighbors[direction] = card_2;
    card_2->neighbors[(direction+2)%4] = card_1;
}

int card__place(struct card *new_card, struct position pos)
{
    if (new_card == NULL)
        exit_on_error("NULL value on card*");

    //TODO : card__place impl
    //For each of the four positions, search card in card_set
    //with search_dicho(set, x, y) -> card
    //update neighbour_list of each card with accurate card position
    //update new card
    //add card to set
    (void) pos;

    return -1;
}

enum card_id card__draw(struct stack *s)
{
    enum card_id *popped_card = stack__pop(s);
    enum card_id returned = *popped_card;
    free(popped_card);
    return returned;
}

void card__set_orientation(struct card *card, enum orientation orientation)
{
    if (card == NULL)
        exit_on_error("NULL value on card*");
    else
        card->orientation = orientation;
}
