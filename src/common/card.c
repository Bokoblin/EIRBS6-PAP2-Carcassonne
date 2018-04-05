#include <stdlib.h>
#include "card.h"
#include "utils.h"

struct card* initialize_card(struct card_type *card_type)
{
  struct card * c = malloc(sizeof(struct card));
  if (c == NULL){
    exit_on_error("Malloc failure on: struct card*");
  }
  c->type = card_type;
  c->orientation = NORTH_TO_NORTH;  
  return c;
}

void card__free(struct card *card)
{
    free(card);
}

enum area_type get_area(struct card *card, enum place place)
{
  if (place == POS_CENTER) return card->type->areas[place-1];
  return card->type->areas[(place-1+3*card->orientation)%12];
}

int matching_cards(struct card *card_1, struct card *card_2, enum direction direction)
{
  for(int i = 0 ; i < 3 ; i++){
    if(get_area(card_1, direction*3+1+i) != get_area(card_2, direction*3+1+i)) return 0; 
  }
  return 1;
}

void card_link(struct card *card_1, struct card *card_2, enum direction direction)
{
  card_1->areas[direction] = card_2;
  card_2->areas[(direction+2)%4] = card_1;
}

int card__place(struct card *new_card, struct card **neighbor_list)
{
    //TODO : card__place impl

    return -1;
}

enum card_id card__draw(struct stack *s)
{
    return stack__pop(s);
}

void set_orientation(struct card *card, enum orientation orientation)
{
  card->orientation = orientation;
}
