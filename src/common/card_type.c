#include "card_type.h"


////////////////////////////////////////////////////////////////////
///     CARD TYPE DEFINITIONS
////////////////////////////////////////////////////////////////////

static struct card_type ct_CARD_MONASTERY_ROAD = {CARD_MONASTERY_ROAD,
        {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, ROAD, FIELD, FIELD, FIELD, ABBEY}};

static struct card_type ct_CARD_MONASTERY_ALONE = {CARD_MONASTERY_ALONE,
        {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, ABBEY}};

static struct card_type ct_CARD_CITY_ALL_SIDES = {CARD_CITY_ALL_SIDES,
        {CITY, CITY, CITY, CITY, CITY, CITY, CITY, CITY, CITY, CITY, CITY, CITY, CITY}};

static struct card_type ct_CARD_ROAD_STRAIGHT_CITY = {CARD_ROAD_STRAIGHT_CITY,
        {CITY, CITY, CITY, FIELD, ROAD, FIELD, FIELD, FIELD, FIELD, FIELD, ROAD, FIELD, ROAD}};

static struct card_type ct_CARD_CITY_ONE_SIDE = {CARD_CITY_ONE_SIDE,
        {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, CITY, CITY, CITY, FIELD}};

static struct card_type ct_CARD_CITY_TUNNEL = {CARD_CITY_TUNNEL,
        {CITY, CITY, CITY, FIELD, FIELD, FIELD, CITY, CITY, CITY, FIELD, FIELD, FIELD, CITY}};

static struct card_type ct_CARD_CITY_TUNNEL_SHLD = {CARD_CITY_TUNNEL_SHLD,
        {CITY, CITY, CITY, FIELD, FIELD, FIELD, CITY, CITY, CITY, FIELD, FIELD, FIELD, CITY}};

static struct card_type ct_CARD_PLAIN_TUNNEL = {CARD_PLAIN_TUNNEL,
        {CITY, CITY, CITY, FIELD, FIELD, FIELD, CITY, CITY, CITY, FIELD, FIELD, FIELD, FIELD}};

static struct card_type ct_CARD_PLAIN_TWO_CITIES = {CARD_PLAIN_TWO_CITIES,
        {CITY, CITY, CITY, CITY, CITY, CITY, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD}};

static struct card_type ct_CARD_ROAD_TURN_RIGHT_CITY = {CARD_ROAD_TURN_RIGHT_CITY,
        {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, FIELD}};

static struct card_type ct_CARD_ROAD_TURN_LEFT_CITY = {CARD_ROAD_TURN_LEFT_CITY,
        {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, FIELD, FIELD, CITY, CITY, CITY, FIELD}};

static struct card_type ct_CARD_JUNCTION_CITY = {CARD_JUNCTION_CITY,
        {CITY, CITY, CITY, FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, INTERSECTION}};

static struct card_type ct_CARD_PLAIN_CITY_SHLD = {CARD_PLAIN_CITY_SHLD,
        {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, FIELD}};

static struct card_type ct_CARD_PLAIN_CITY = {CARD_PLAIN_CITY,
        {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, ROAD}};

static struct card_type ct_CARD_PLAIN_CITY_ROAD_SHLD = {CARD_PLAIN_CITY_ROAD_SHLD,
        {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, FIELD}};

static struct card_type ct_CARD_PLAIN_CITY_ROAD = {CARD_PLAIN_CITY_ROAD,
        {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, FIELD}};

static struct card_type ct_CARD_CITY_THREE_SHLD = {CARD_CITY_THREE_SHLD,
        {CITY, CITY, CITY, FIELD, FIELD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, CITY}};

static struct card_type ct_CARD_CITY_THREE = {CARD_CITY_THREE,
        {CITY, CITY, CITY, FIELD, FIELD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, CITY}};

static struct card_type ct_CARD_CITY_THREE_ROAD_SHLD = {CARD_CITY_THREE_ROAD_SHLD,
        {CITY, CITY, CITY, FIELD, ROAD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, CITY}};

static struct card_type ct_CARD_CITY_THREE_ROAD = {CARD_CITY_THREE_ROAD,
        {CITY, CITY, CITY, FIELD, ROAD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, CITY}};

static struct card_type ct_CARD_ROAD_STRAIGHT = {CARD_ROAD_STRAIGHT,
        {FIELD, FIELD, FIELD, FIELD, ROAD, FIELD, FIELD, FIELD, FIELD, FIELD, ROAD, FIELD, ROAD}};

static struct card_type ct_CARD_ROAD_TURN = {CARD_ROAD_TURN,
        {FIELD, FIELD, FIELD, FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, FIELD, FIELD, ROAD}};

static struct card_type ct_CARD_JUNCTION_THREE = {CARD_JUNCTION_THREE,
        {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, FIELD, FIELD, INTERSECTION}};

static struct card_type ct_CARD_JUNCTION_FOUR = {CARD_JUNCTION_FOUR,
        {FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, FIELD, ROAD, FIELD, INTERSECTION}};

static struct card_type ct_LAST_CARD = {LAST_CARD,
        {INVALID_AREA, INVALID_AREA, INVALID_AREA, INVALID_AREA, INVALID_AREA, INVALID_AREA, INVALID_AREA,
                INVALID_AREA, INVALID_AREA, INVALID_AREA, INVALID_AREA, INVALID_AREA, INVALID_AREA}};


////////////////////////////////////////////////////////////////////
///     CARD TYPE FUNCTIONS
////////////////////////////////////////////////////////////////////

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
        case CARD_CITY_TUNNEL :
            ct = ct_CARD_CITY_TUNNEL;
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
            ct = ct_LAST_CARD;
            break;
    }
    return ct;
}
