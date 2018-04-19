#include <stdlib.h>
#include "card.h"
#include "utils.h"


struct card* card__init(enum card_id card_id)
{
    struct card *c = safe_malloc(sizeof(struct card));

    c->type = card__id_to_type(card_id);
    c->orientation = DEFAULT_ORIENTATION;
    c->pos.x = 999; //meaning invalid
    c->pos.y = 999;

    for (int i = 0; i < 4; i++)
        c->neighbors[i] = NULL;

    return c;
}

void card__free(struct card *card)
{
    free(card);
}

enum area_type card__get_area(struct card *card, enum place place)
{
    assert_not_null(card, __func__, "card parameter");

    if (place == POS_CENTER)
        return card->type.areas[place-1];

    return card->type.areas[(12-(place-1+3*card->orientation))%12];
}

unsigned int card__get_neighbour_number(struct card *card)
{
    assert_not_null(card, __func__, "card parameter");

    unsigned int cpt = 0;

    for (int i = 0; i < 4; i++)
        if (card->neighbors[i] != NULL)
            cpt++;

    return cpt;
}

int card__are_matching(struct card *c1, struct card *c2)
{
    assert_not_null(c1, __func__, "c1 parameter");
    assert_not_null(c2, __func__, "c2 parameter");

    return card__are_matching_direction(c1, c2, NORTH)
            || card__are_matching_direction(c1, c2, SOUTH)
            || card__are_matching_direction(c1, c2, WEST)
            || card__are_matching_direction(c1, c2, EAST);
}

int card__are_matching_direction(struct card *c1, struct card *c2, enum direction direction)
{
    assert_not_null(c1, __func__, "c1 parameter");
    assert_not_null(c2, __func__, "c2 parameter");

    for (int i = 0; i < 3; i++) {
        unsigned int place_index = (direction * 3 + 1 + i);
        if (place_index > LAST_POS)
            exit_on_error("Out of range place");

        enum place p = (enum place) place_index;
        //FIXME: temp
        enum area_type c1_a = card__get_area(c1, p);
        enum area_type c2_a = card__get_area(c2, (p + 2) % DIRECTION_NUMBER);
        if (c1_a != c2_a)
            return false; //on matching failure
    }

    return true;
}

void card__link_at_direction(struct card *c1, struct card *c2, enum direction direction)
{
    assert_not_null(c1, __func__, "c1 parameter");
    assert_not_null(c2, __func__, "c2 parameter");

    if (direction > DIRECTION_NUMBER)
        exit_on_error("Out of range direction");

    c1->neighbors[direction] = c2;
    c2->neighbors[(direction + 2) % DIRECTION_NUMBER] = c1;
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
    assert_not_null(card, __func__, "card parameter");
    card->orientation = orientation;
}
