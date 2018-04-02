#include <stdlib.h>
#include "cards.h"

#define DEFAULT_FREE_POSITIONS_SIZE 8

struct board* initialize_board ()
{
  struct board *b = malloc(sizeof(struct board));
  b->first_card = empty_card(CARD_JUNCTION_THREE);
  b->fp_capacity = DEFAULT_FREE_POSITIONS_SIZE;
  b->free_positions = malloc(sizeof(struct card) * b->fp_capacity);
  b->free_positions[0] = b->first_card;
  //TODO initialize free_positions with the four cards that match the first one
  //Baptiste : I though the free_positions array was the list of cards next to free spaces (extremity cards)
  return b;
}

struct card* empty_card (enum card_id id)
{
  struct card * c = malloc(sizeof(struct card));
  c->id = id;
  c->north = NULL;
  c->south = NULL;
  c->east = NULL;
  c->west = NULL;
  c->type_north_east = UNVALID_AREA;
  c->type_north      = UNVALID_AREA;
  c->type_north_west = UNVALID_AREA;
  c->type_west_north = UNVALID_AREA;
  c->type_west       = UNVALID_AREA;
  c->type_west_south = UNVALID_AREA;
  c->type_south_west = UNVALID_AREA;
  c->type_south      = UNVALID_AREA;
  c->type_south_east = UNVALID_AREA;
  c->type_east_north = UNVALID_AREA;
  c->type_east       = UNVALID_AREA;
  c->type_east_south = UNVALID_AREA;
  c->type_center     = UNVALID_AREA;
  return c;
}

void free_board (struct board* game)
{
  for (unsigned int i = 0; i < game->fp_capacity; i++){
    free_card(game->free_positions[i]);
  }
  //WARNING : might be a bug if the first card was freed by the free_positions array
  free(game->first_card);
  free(game->free_positions);
  free(game);
}

void free_card (struct card* card)
{
  free(card);
}

void clockwise_rotation (struct card *card)
{
  //TODO : Also rotate the areas
  struct card * tmp_north = card->north;
  card->north = card->west;
  card->west = card->south;
  card->south = card->east;
  card->east = tmp_north;  
}

void trigo_rotation (struct card *card)
{
  //TODO : Also rotate the areas
  struct card * tmp_north = card->north;
  card->north = card->east;
  card->east = card->south;
  card->south = card->west;
  card->west = tmp_north;
}

int place(struct card* new_card, struct card** neighbor_list);

enum card_id draw_card(struct pile* p);
