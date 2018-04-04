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
