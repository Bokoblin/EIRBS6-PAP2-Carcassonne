#include <stdlib.h>
#include "card.h"
#include "utils.h"

struct card_type card__id_to_type(enum card_id id)
{
    struct card_type ct;
    switch (id) {
    case CARD_MONASTERY_ROAD:
        ct = ct_CARD_MONASTERY_ROAD;
        break;
    case CARD_MONASTERY_ALONE :
        ct = ct_CARD_MONASTERY_ALONE;
        break;
    case CARD_CITY_ALL_SIDES :
        ct = ct_CARD_CITY_ALL_SIDES;
        break;
    case CARD_ROAD_STRAIGHT_CITY :
        ct = ct_CARD_ROAD_STRAIGHT_CITY;
        break;
    case CARD_CITY_ONE_SIDE :
        ct = ct_CARD_CITY_ONE_SIDE;
        break;
    case CARD_CITY_TUNNEL_SHLD :
        ct = ct_CARD_CITY_TUNNEL_SHLD;
        break;
    case CARD_PLAIN_TUNNEL:
        ct = ct_CARD_PLAIN_TUNNEL;
        break;
    case CARD_PLAIN_TWO_CITIES:
        ct = ct_CARD_PLAIN_TWO_CITIES;
        break;
    case CARD_ROAD_TURN_RIGHT_CITY :
        ct = ct_CARD_ROAD_TURN_RIGHT_CITY;
        break;
    case CARD_ROAD_TURN_LEFT_CITY :
        ct = ct_CARD_ROAD_TURN_LEFT_CITY;
        break;
    case CARD_JUNCTION_CITY :
        ct = ct_CARD_JUNCTION_CITY;
        break;
    case CARD_PLAIN_CITY_SHLD :
        ct = ct_CARD_PLAIN_CITY_SHLD;
        break;
    case CARD_PLAIN_CITY :
        ct = ct_CARD_PLAIN_CITY;
        break;
    case CARD_PLAIN_CITY_ROAD_SHLD :
        ct = ct_CARD_PLAIN_CITY_ROAD_SHLD;
        break;
    case CARD_PLAIN_CITY_ROAD :
        ct = ct_CARD_PLAIN_CITY_ROAD;
        break;
    case CARD_CITY_THREE_SHLD:
        ct = ct_CARD_CITY_THREE_SHLD;
        break;
    case CARD_CITY_THREE :
        ct = ct_CARD_CITY_THREE;
        break;
    case CARD_CITY_THREE_ROAD_SHLD :
        ct = ct_CARD_CITY_THREE_ROAD_SHLD;
        break;
    case CARD_CITY_THREE_ROAD:
        ct = ct_CARD_CITY_THREE_ROAD;
        break;
    case CARD_ROAD_STRAIGHT :
        ct = ct_CARD_ROAD_STRAIGHT;
        break;
    case CARD_ROAD_TURN :
        ct = ct_CARD_ROAD_TURN;
        break;
    case CARD_JUNCTION_THREE :
        ct = ct_CARD_JUNCTION_THREE;
        break;
    case CARD_JUNCTION_FOUR :
        ct = ct_CARD_JUNCTION_FOUR;
        break;
    default:
        exit_on_error("Unknown card type");
        break;
    }
    return ct;
}

struct card* card__empty(enum card_id card_id)
{
    struct card * c = malloc(sizeof(struct card));
    if (c == NULL) {
        exit_on_error("Malloc failure on: struct card*");
    } else {
        c->type = card__id_to_type(card_id);
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
    if (card == NULL)
        exit_on_error("NULL value on card*");

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

int card__place(struct card *new_card, struct card **neighbor_list)
{
    if (new_card == NULL)
        exit_on_error("NULL value on card*");

    //TODO : card__place impl
    (void) neighbor_list;

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
