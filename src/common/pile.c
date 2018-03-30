#ifndef __PILE_C__
#define __PILE_C__

#include <stdlib.h>
#include "pile.h"

#define DEFAULT_PILE_CAPACITY 72

struct pile{
  void *a;
  int capacity;
  int head;
};



struct pile *pile__empty(){
  struct pile *p = malloc(sizeof(struct pile));
  p->capacity = DEFAULT_PILE_CAPACITY;
  p->head = 0;
  p->a = malloc(sizeof(void*)*p->capacity);
}


int pile__stack(struct pile *pile, void *element);


void *pile__unstack(struct pile *pile);


void pile__free(struct pile *pile){
  free(p->a);
  free(p);
  return 0;
}

#endif
