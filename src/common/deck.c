#include <stdio.h>
#include "com_func_ptrs.h"
#include "common_interface.h"
#include "deck.h"
#include "utils.h"

struct stack* init_deck()
{
    struct stack *s = stack__empty(&cardid_copy_op, &cardid_delete_op, &cardid_debug_op);

    enum card_id card_CARD_ROAD_STRAIGHT_CITY = CARD_ROAD_STRAIGHT_CITY;
    enum card_id card_CARD_CITY_ALL_SIDES = CARD_CITY_ALL_SIDES;
    enum card_id card_CARD_CITY_TUNNEL = CARD_CITY_TUNNEL;
    enum card_id card_CARD_CITY_THREE_SHLD = CARD_CITY_THREE_SHLD;
    enum card_id card_CARD_CITY_THREE_ROAD = CARD_CITY_THREE_ROAD;
    enum card_id card_CARD_JUNCTION_FOUR = CARD_JUNCTION_FOUR;
    enum card_id card_CARD_MONASTERY_ROAD = CARD_MONASTERY_ROAD;
    enum card_id card_CARD_CITY_TUNNEL_SHLD = CARD_CITY_TUNNEL_SHLD;
    enum card_id card_CARD_PLAIN_TWO_CITIES = CARD_PLAIN_TWO_CITIES;
    enum card_id card_CARD_PLAIN_CITY_SHLD = CARD_PLAIN_CITY_SHLD;
    enum card_id card_CARD_PLAIN_CITY_ROAD_SHLD = CARD_PLAIN_CITY_ROAD_SHLD;
    enum card_id card_CARD_CITY_THREE_ROAD_SHLD = CARD_CITY_THREE_ROAD_SHLD;
    enum card_id card_CARD_PLAIN_TUNNEL = CARD_PLAIN_TUNNEL;
    enum card_id card_CARD_ROAD_TURN_RIGHT_CITY = CARD_ROAD_TURN_RIGHT_CITY;
    enum card_id card_CARD_ROAD_TURN_LEFT_CITY = CARD_ROAD_TURN_LEFT_CITY;
    enum card_id card_CARD_JUNCTION_CITY = CARD_JUNCTION_CITY;
    enum card_id card_CARD_PLAIN_CITY = CARD_PLAIN_CITY;
    enum card_id card_CARD_PLAIN_CITY_ROAD = CARD_PLAIN_CITY_ROAD;
    enum card_id card_CARD_CITY_THREE = CARD_CITY_THREE;
    enum card_id card_CARD_MONASTERY_ALONE = CARD_MONASTERY_ALONE;
    enum card_id card_CARD_JUNCTION_THREE = CARD_JUNCTION_THREE;
    enum card_id card_CARD_CITY_ONE_SIDE = CARD_CITY_ONE_SIDE;
    enum card_id card_CARD_ROAD_STRAIGHT = CARD_ROAD_STRAIGHT;
    enum card_id card_CARD_ROAD_TURN = CARD_ROAD_TURN;
    
    stack__push(s, &card_CARD_CITY_ALL_SIDES);
    stack__push(s, &card_CARD_CITY_TUNNEL);
    stack__push(s, &card_CARD_CITY_THREE_SHLD);
    stack__push(s, &card_CARD_CITY_THREE_ROAD);
    stack__push(s, &card_CARD_JUNCTION_FOUR);

    for (size_t i = 0; i < 2; i++) {
        stack__push(s, &card_CARD_MONASTERY_ROAD);
        stack__push(s, &card_CARD_CITY_TUNNEL_SHLD);
        stack__push(s, &card_CARD_PLAIN_TWO_CITIES);
        stack__push(s, &card_CARD_PLAIN_CITY_SHLD);
        stack__push(s, &card_CARD_PLAIN_CITY_ROAD_SHLD);
        stack__push(s, &card_CARD_CITY_THREE_ROAD_SHLD);
        stack__push(s, &card_CARD_JUNCTION_CITY);
    }

    for (size_t i = 0; i < 3; i++) {
        stack__push(s, &card_CARD_PLAIN_TUNNEL);
        stack__push(s, &card_CARD_ROAD_TURN_RIGHT_CITY);
        stack__push(s, &card_CARD_ROAD_TURN_LEFT_CITY);
        stack__push(s, &card_CARD_PLAIN_CITY);
        stack__push(s, &card_CARD_PLAIN_CITY_ROAD);
        stack__push(s, &card_CARD_CITY_THREE);
    }

    for (size_t i = 0; i < 4; i++) {
        stack__push(s, &card_CARD_MONASTERY_ALONE);
        stack__push(s, &card_CARD_JUNCTION_THREE);
        stack__push(s, &card_CARD_ROAD_STRAIGHT_CITY);
    }

    for (size_t i = 0; i < 5; i++) {
        stack__push(s, &card_CARD_CITY_ONE_SIDE);
    }

    for (size_t i = 0; i < 8; i++) {
        stack__push(s, &card_CARD_ROAD_STRAIGHT);
    }

    for (size_t i = 0; i < 9; i++) {
        stack__push(s, &card_CARD_ROAD_TURN);
    }

    stack__mix(s);
    stack__push(s, &card_CARD_JUNCTION_CITY); //First card to always place in first

    return s;
}
