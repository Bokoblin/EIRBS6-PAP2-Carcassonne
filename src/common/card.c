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

struct position card__get_position_at_direction(struct card *card, enum direction direction)
{
    assert_not_null(card, __func__, "card parameter");

    if (card->neighbors[direction] != NULL)
        return card->neighbors[direction]->pos;
    else {
        struct position p_array[4]  = {
                //We consider growing y is going North and growing x going toward East
                { card->pos.x,      card->pos.y + 1 }, //North neighbour
                { card->pos.x - 1,  card->pos.y     }, //West neighbour
                { card->pos.x,      card->pos.y - 1 }, //South neighbour
                { card->pos.x + 1,  card->pos.y     }  //East neighbour
        };
        return p_array[direction];
    }
}

int card__are_matching_free_side(struct card *c1, struct card *c2)
{
    assert_not_null(c1, __func__, "c1 parameter");
    assert_not_null(c2, __func__, "c2 parameter");

    for (int i = 0; i < DIRECTION_NUMBER; i++)
        for (int j = 0; j < DIRECTION_NUMBER; j++)
            if (card__are_matching_directions(c1, c2, (enum direction) i, (enum direction) j)
                    && c1->neighbors[i] == NULL && c2->neighbors[j] == NULL)
                return true;
    return false;
}

//NOTE: This function is deprecated
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

int card__are_matching_directions(struct card *c1, struct card *c2, enum direction d1, enum direction d2)
{
    assert_not_null(c1, __func__, "c1 parameter");
    assert_not_null(c2, __func__, "c2 parameter");

    if (c1 == c2)
        return false;

    if (d1 < 0 || d2 < 0 || d1 > DIRECTION_NUMBER || d2 > DIRECTION_NUMBER)
        return false;

    for (int i = 0; i < 3; i++) {
        int c1_area_index = (3 * d1 + 3 * c1->orientation + i) % 12;
        int c2_area_index = (3 * d2 + 3 * c2->orientation + 2 - i) % 12;

        if (c1_area_index > LAST_POS || c2_area_index > LAST_POS)
            return false;

        enum area_type c1_a = c1->type.areas[c1_area_index];
        enum area_type c2_a = c2->type.areas[c2_area_index];

        if (c1_a != c2_a)
            return false; //on matching failure
    }

    return true;
}

//NOTE: This function is deprecated
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

int card__link_at_directions(struct card *c1, struct card *c2, enum direction d1, enum direction d2)
{
    assert_not_null(c1, __func__, "c1 parameter");
    assert_not_null(c2, __func__, "c2 parameter");

    if (c1 == c2)
        return !SUCCESS;

    if (d1 < 0 || d2 < 0 || d1 > DIRECTION_NUMBER || d2 > DIRECTION_NUMBER)
        return !SUCCESS;

    if (c1->neighbors[d1] != NULL || c2->neighbors[d2] != NULL)
        return !SUCCESS;

    c1->neighbors[d1] = c2;
    c2->neighbors[d2] = c1;

    return SUCCESS;
}

enum card_id card__draw(struct stack *s)
{
    enum card_id *popped_card = stack__pop(s);
    enum card_id returned = *popped_card;
    free(popped_card);
    return returned;
}
