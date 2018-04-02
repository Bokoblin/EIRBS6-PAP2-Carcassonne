#include <stdlib.h>
#include "card.h"
#include "utils.h"

struct card* card__empty(enum card_id id)
{
    struct card * c = malloc(sizeof(struct card));
    if (c == NULL) {
        exit_on_error("Malloc failure on: struct card*");
    } else {
        c->id = id;
        c->north = NULL;
        c->south = NULL;
        c->east = NULL;
        c->west = NULL;
        c->type_north_east = INVALID_AREA;
        c->type_north      = INVALID_AREA;
        c->type_north_west = INVALID_AREA;
        c->type_west_north = INVALID_AREA;
        c->type_west       = INVALID_AREA;
        c->type_west_south = INVALID_AREA;
        c->type_south_west = INVALID_AREA;
        c->type_south      = INVALID_AREA;
        c->type_south_east = INVALID_AREA;
        c->type_east_north = INVALID_AREA;
        c->type_east       = INVALID_AREA;
        c->type_east_south = INVALID_AREA;
        c->type_center     = INVALID_AREA;
    }

    return c;
}

void card__free(struct card *card)
{
    free(card);
}

int card__place(struct card *new_card, struct card **neighbor_list)
{
    //TODO
}

enum card_id card__draw(struct stack *s)
{
    return stack__pop(s);
}

void card__clockwise_rotation(struct card *card)
{
    //TODO : Also rotate the areas
    struct card * tmp_north = card->north;
    card->north = card->west;
    card->west = card->south;
    card->south = card->east;
    card->east = tmp_north;
    enum area_type tmp = card->type_north;
    card->type_north = card->type_north_west;
    card->type_north_west = card->type_west_north;
    card->type_west_north = card->type_west;
    card->type_west = card->type_west_south;
    card->type_west_south = card->type_south_west;
    card->type_south_west = card->type_south;
    card->type_south = card->type_south_east;
    card->type_south_east = card->type_east_south;
    card->type_east_south = card->type_east;
    card->type_east = card->type_east_north;
    card->type_east_north = card->type_north_east;
    card->type_north_east = tmp;
}

void card__trigo_rotation(struct card *card)
{
    //TODO : Also rotate the areas
    struct card * tmp_north = card->north;
    card->north = card->east;
    card->east = card->south;
    card->south = card->west;
    card->west = tmp_north;
    enum area_type tmp = card->type_north;
    card->type_north = card->type_north_east;
    card->type_north_east = card->type_east_north;
    card->type_east_north = card->type_east;
    card->type_east = card->type_east_south;
    card->type_east_south = card->type_south_east;
    card->type_south_east = card->type_south;
    card->type_south = card->type_south_west;
    card->type_south_west = card->type_west_south;
    card->type_west_south = card->type_west;
    card->type_west = card->type_west_north;
    card->type_west_north = card->type_north_west;
    card->type_north_west = tmp;
}
