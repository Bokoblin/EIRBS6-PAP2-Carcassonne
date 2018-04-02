#ifndef __PILE_C__
#define __PILE_C__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pile.h"

#define DEFAULT_PILE_CAPACITY 2

struct pile{
  enum card_id *a;
  unsigned int capacity;
  unsigned int head;
};



struct pile *pile__empty ()
{
  struct pile *p = malloc(sizeof(struct pile));
  p->capacity = DEFAULT_PILE_CAPACITY;
  p->head = 0;
  p->a = malloc(sizeof(enum card_id) * (p->capacity));
  return p;
}


int pile__is_empty (struct pile *pile)
{
  return (pile->head == 0);
}

int pile__stack (struct pile *pile, enum card_id element)
{
  assert(pile->a != NULL);
  if (element > LAST_CARD)
    return -1;

  /* Adjust capacity if necessary */
  if (pile->head == pile->capacity){
    pile->capacity = pile->capacity * 2;
    pile->a = realloc(pile->a, sizeof(enum card_id) * pile->capacity);
    assert(pile->a != NULL);
  }

  pile->a[pile->head] = element;
  pile->head++;
  return 0;
}


enum card_id pile__unstack (struct pile *pile)
{
  assert(pile->a != NULL);
  if (pile__is_empty(pile))
    return LAST_CARD;

  /* Adjust capacity if necessary */
  if ((pile->head <= pile->capacity / 4) && (pile->capacity > DEFAULT_PILE_CAPACITY)){
    pile->capacity = pile->capacity / 2;
    pile->a = realloc(pile-> a, sizeof(enum card_id) * pile->capacity);
    assert(pile->a != NULL);
  }

  pile->head--;
  return pile->a[pile->head];
}


unsigned int pile__length (struct pile *pile)
{
  return (pile->head);
}


void pile__free (struct pile *pile)
{
  free(pile->a);
  free(pile);
}


void pile__debug (struct pile *pile)
{
  if (pile__is_empty(pile)){
    printf("Pile is empty.");
    return;
  }

  for (unsigned int i = 0; i < pile->head; i++){
    printf("%d ", pile->a[i]);
  }
}

#endif
