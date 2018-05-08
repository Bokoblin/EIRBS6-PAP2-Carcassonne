#include "card.h"
#include "utils.h"


struct card* card__init(enum card_id card_id)
{
    struct card *c = safe_malloc(sizeof(struct card));

    c->type = card__id_to_type(card_id);
    c->direction = DEFAULT_DIRECTION;
    c->pos.x = INVALID_POSITION;
    c->pos.y = INVALID_POSITION;

    for (int i = 0; i < SIDES_NUMBER; i++)
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

    return card->type.areas[(place - 1 + 3 * card->direction) % 12];
}

unsigned int card__get_neighbour_number(struct card *card)
{
    assert_not_null(card, __func__, "card parameter");

    unsigned int cpt = 0;

    for (int i = 0; i < SIDES_NUMBER; i++)
        if (card->neighbors[i] != NULL)
            cpt++;

    return cpt;
}

struct position card__get_position_at_side(struct card *card, enum card_side concerned_side)
{
    assert_not_null(card, __func__, "card parameter");

    if (card->neighbors[concerned_side] != NULL)
        return card->neighbors[concerned_side]->pos;
    else {
        struct position p_array[4]  = {
                //We consider growing y is going North and growing x going toward East
                { card->pos.x,      card->pos.y + 1 }, //North neighbour
                { card->pos.x - 1,  card->pos.y     }, //West neighbour
                { card->pos.x,      card->pos.y - 1 }, //South neighbour
                { card->pos.x + 1,  card->pos.y     }  //East neighbour
        };
        return p_array[concerned_side];
    }
}

int card__are_matching_free_side(struct card *c1, struct card *c2)
{
    assert_not_null(c1, __func__, "c1 parameter");
    assert_not_null(c2, __func__, "c2 parameter");

    for (int c1_side_index = 0; c1_side_index < SIDES_NUMBER; c1_side_index++)
        for (int c2_side_index = 0; c2_side_index < SIDES_NUMBER; c2_side_index++)
            if (card__are_matching_sides(c1, c2, (enum card_side) c1_side_index, (enum card_side) c2_side_index)
                    && c1->neighbors[c1_side_index] == NULL && c2->neighbors[c2_side_index] == NULL)
                return true;
    return false;
}

int card__are_matching_sides(struct card *c1, struct card *c2, enum card_side s1, enum card_side s2)
{
    assert_not_null(c1, __func__, "c1 parameter");
    assert_not_null(c2, __func__, "c2 parameter");

    if (c1 == c2)
        return false;

    if (s1 < 0 || s2 < 0 || s1 >= SIDES_NUMBER || s2 >= SIDES_NUMBER)
        return false;

    for (int i = 0; i < SIDE_SUBAREAS_NUMBER; i++) {
        //TODO: check correctness, move validation failure might come from here
        int c1_area_index = (SIDE_SUBAREAS_NUMBER * s1 + SIDE_SUBAREAS_NUMBER * c1->direction + i) % MAX_SIDE_ZONES;
        int c2_area_index = (SIDE_SUBAREAS_NUMBER * s2 + SIDE_SUBAREAS_NUMBER * c2->direction + 2 - i) % MAX_SIDE_ZONES;

        if (c1_area_index > LAST_POS || c2_area_index > LAST_POS)
            return false;

        enum area_type c1_a = c1->type.areas[c1_area_index];
        enum area_type c2_a = c2->type.areas[c2_area_index];

        //Check sub areas matching
        if (c1_a != c2_a)
            return false;
    }

    return true;
}

int card__link_at_sides(struct card *c1, struct card *c2, enum card_side s1, enum card_side s2)
{
    assert_not_null(c1, __func__, "c1 parameter");
    assert_not_null(c2, __func__, "c2 parameter");

    if (c1 == c2)
        return false;

    if (s1 < 0 || s2 < 0 || s1 >= SIDES_NUMBER || s2 >= SIDES_NUMBER)
        return false;

    if (c1->neighbors[s1] != NULL || c2->neighbors[s2] != NULL)
        return false;

    c1->neighbors[s1] = c2;
    c2->neighbors[s2] = c1;

    return true;
}

enum card_id card__draw(struct stack *s)
{
    enum card_id *popped_card = stack__pop(s);
    enum card_id returned = *popped_card;
    free(popped_card);
    return returned;
}
