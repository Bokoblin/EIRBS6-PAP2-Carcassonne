#include "card.h"
#include "utils.h"


struct card* card__init(enum card_id card_id)
{
    struct card *c = safe_malloc(sizeof(struct card));

    c->type = card__id_to_type(card_id);
    c->orientation = DEFAULT_ORIENTATION;
    c->pos.x = INVALID_POSITION;
    c->pos.y = INVALID_POSITION;

    for (int i = 0; i < DIRECTION_NUMBER; i++)
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

    if (place >= LAST_POS)
        return INVALID_AREA;

    if (place == POS_CENTER)
        return card->type.areas[place-1];

    return card->type.areas[(place - 1 + 3 * card->orientation) % 12];
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

    if (c1 == c2)
        return false;

    for (int i = 0; i < 3; i++) {
        enum direction opposite_dir = (direction + 2) % DIRECTION_NUMBER;
        int c1_area_index = (3 * direction + 3 * c1->orientation + i) % 12;
        int c2_area_index = (3 * opposite_dir + 3 * c2->orientation + 2 - i) % 12;

        if (c1_area_index > LAST_POS || c2_area_index > LAST_POS)
            return false;

        enum area_type c1_a = c1->type.areas[c1_area_index];
        enum area_type c2_a = c2->type.areas[c2_area_index];

        if (c1_a != c2_a)
            return false; //on matching failure
    }

    return true;
}

int card__link_at_direction(struct card *c1, struct card *c2, enum direction direction)
{
    assert_not_null(c1, __func__, "c1 parameter");
    assert_not_null(c2, __func__, "c2 parameter");

    if (c1 == c2)
        return !SUCCESS;

    if (direction > DIRECTION_NUMBER)
        return !SUCCESS;

    c1->neighbors[direction] = c2;
    c2->neighbors[(direction + 2) % DIRECTION_NUMBER] = c1;

    return SUCCESS;
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
