#include <stdlib.h>
#include "cards.h"


struct board* initialize_board()
{
  struct board *b = malloc(sizeof(struct board));
  struct card *first_card = empty_card(CARD_JUNCTION_THREE);
  //TODO initialize free_positions with the four cards that match the first one
  return b;
}

struct card* empty_card(enum card_id id)
{
  struct card * c = malloc(sizeof(struct card));
  c->id = id;
  c->north = NULL;
  c->south = NULL;
  c->east = NULL;
  c->west = NULL;
  return c;
}

void free_board(struct board* game)
{
   
}

void free_card(struct card* card)
{
  free(card);
}

void clockwise_rotation(struct card *card)
{
  struct card * tmp_card = card->north;
  card->north = card->west;
  card->west = card->south;
  card->south = card->east;
  card-east = card->tmp_card;
  
}

void trigo_rotation(struct card *card)
{

}
