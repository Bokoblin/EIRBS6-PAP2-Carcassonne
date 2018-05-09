#include "card.h"
#include "utils.h"


struct card* card__init(enum card_id card_id)
{
    struct card *c = safe_malloc(sizeof(struct card));

    c->type = card__id_to_type(card_id);
    c->direction = DEFAULT_DIRECTION;
    c->pos.x = INVALID_POSITION;
    c->pos.y = INVALID_POSITION;

    for (int i = 0; i < NB_DIRECTIONS; i++)
        c->neighbors[i] = NULL;

    return c;
}

void card__free(struct card *card)
{
    free(card);
}

enum area_type card__get_relative_area(struct card *card, enum place place)
{
    assert_not_null(card, __func__, "card parameter");

    if (place >= LAST_POS)
        return INVALID_AREA;

    if (place == POS_CENTER)
        return card->type.areas[place-1];

    return card->type.areas[(place - PLACE_SHIFT + (NB_SUB_DIRECTIONS * card->direction)) % MAX_SIDE_ZONES];
}

//!!! ALPHA FUNCTION !!!
enum area_type card__get_absolute_area(struct card *card, enum place place)
{
    assert_not_null(card, __func__, "card parameter");

    if (place >= LAST_POS)
        return INVALID_AREA;

    if (place == POS_CENTER)
        return card->type.areas[place-1];

    //Get absolute direction and not where direction is on card's sides
    return card->type.areas[(MAX_SIDE_ZONES - (NB_SUB_DIRECTIONS * card->direction) + place) % MAX_ZONES];
}

unsigned int card__get_neighbour_number(struct card *card)
{
    assert_not_null(card, __func__, "card parameter");

    unsigned int cpt = 0;

    for (int i = 0; i < NB_DIRECTIONS; i++)
        if (card->neighbors[i] != NULL)
            cpt++;

    return cpt;
}

struct position card__get_position_at_direction(struct card *card, enum direction dir)
{
    assert_not_null(card, __func__, "card parameter");

    if (card->neighbors[dir] != NULL)
        return card->neighbors[dir]->pos;
    else {
        struct position p_array[4]  = {
                //We consider growing y is going North and growing x going toward East
                { card->pos.x,      card->pos.y + 1 }, //North neighbour
                { card->pos.x - 1,  card->pos.y     }, //West neighbour
                { card->pos.x,      card->pos.y - 1 }, //South neighbour
                { card->pos.x + 1,  card->pos.y     }  //East neighbour
        };
        return p_array[dir];
    }
}

int card__are_matching_free_direction(struct card *c1, struct card *c2)
{
    assert_not_null(c1, __func__, "c1 parameter");
    assert_not_null(c2, __func__, "c2 parameter");

    for (enum direction c1_dir_index = NORTH; c1_dir_index < NB_DIRECTIONS; c1_dir_index++)
        for (enum direction c2_dir_index = NORTH; c2_dir_index < NB_DIRECTIONS; c2_dir_index++)
            if (card__are_matching_directions(c1, c2, c1_dir_index, c2_dir_index)
                    && c1->neighbors[c1_dir_index] == NULL
                    && c2->neighbors[c2_dir_index] == NULL)
                return true;
    return false;
}

int card__are_matching_directions(struct card *c1, struct card *c2, enum direction d1, enum direction d2)
{
    assert_not_null(c1, __func__, "c1 parameter");
    assert_not_null(c2, __func__, "c2 parameter");

    if (c1 == c2)
        return false;

    if (d1 < 0 || d2 < 0 || d1 >= NB_DIRECTIONS || d2 >= NB_DIRECTIONS)
        return false;

    for (int i = 0; i < NB_SUB_DIRECTIONS; i++) {
        //TODO: check correctness, move validation failure might come from here
        int c1_area_index = (NB_SUB_DIRECTIONS * d1 + NB_SUB_DIRECTIONS * c1->direction + i) % MAX_SIDE_ZONES;      //NOTE: LEGACY - half working
        int c2_area_index = (NB_SUB_DIRECTIONS * d2 + NB_SUB_DIRECTIONS * c2->direction + 2 - i) % MAX_SIDE_ZONES;  //NOTE: LEGACY - half working
        //int c2_area_index = (NB_SUB_DIRECTIONS * d2 + NB_SUB_DIRECTIONS * (4 - c2->direction) + 2 - i) % MAX_SIDE_ZONES;  //NOTE: NEW - alpha testing
        //enum place c1_area_index = (enum place) (PLACE_SHIFT + (NB_SUB_DIRECTIONS * d1) + i);        //NOTE: NEW - alpha testing
        //enum place c2_area_index = (enum place) (PLACE_SHIFT + (NB_SUB_DIRECTIONS * d2) + 2 - i);    //NOTE: NEW - alpha testing

        if (c1_area_index > LAST_POS || c2_area_index > LAST_POS)
            return false;

        enum area_type c1_a = c1->type.areas[c1_area_index];                //NOTE: LEGACY - half working
        enum area_type c2_a = c2->type.areas[c2_area_index];                //NOTE: LEGACY - half working
        //enum area_type c1_a = card__get_absolute_area(c1, c1_area_index); //NOTE: NEW - alpha testing
        //enum area_type c2_a = card__get_absolute_area(c2, c2_area_index); //NOTE: NEW - alpha testing

        //Check sub areas matching
        if (c1_a != c2_a)
            return false;
    }

    return true;
}

int card__link_at_directions(struct card *c1, struct card *c2, enum direction d1, enum direction d2)
{
    assert_not_null(c1, __func__, "c1 parameter");
    assert_not_null(c2, __func__, "c2 parameter");

    if (c1 == c2)
        return false;

    if (d1 < 0 || d2 < 0 || d1 >= NB_DIRECTIONS || d2 >= NB_DIRECTIONS)
        return false;

    if (c1->neighbors[d1] != NULL || c2->neighbors[d2] != NULL)
        return false;

    c1->neighbors[d1] = c2;
    c2->neighbors[d2] = c1;

    return true;
}

void card__unlink_neighbours(struct card* c)
{
    for (unsigned int i = 0; i < NB_DIRECTIONS; i++) {
        if (c->neighbors[i] != NULL) {
            c->neighbors[i]->neighbors[(i + 2) % NB_DIRECTIONS] = NULL;
            c->neighbors[i] = NULL;
        }
    }
}
