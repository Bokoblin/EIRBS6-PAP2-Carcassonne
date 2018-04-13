#ifndef __SET_H__
#define __SET_H__

#include <stddef.h>

#define SUCCESS  0

struct set;

struct set *set__empty(void* (*copy) ( void* x),
		       void (*my_delete) (void*),
		       int (*compare) (void* x,  void* y));

int set__is_empty( const struct set *s);

int set__find( const struct set *s, void* e);
size_t set__size( const struct set *s);

int set__add( struct set *s, void* e);
int set__remove( struct set *s, void* e);

void set__free(struct set *s);

struct set *set__filter(const struct set *set, int (*filter) (const void*));

void set__debug_data(const struct set *set, void (*print_data) (const void*));

/*
REMARQUE :
IL FAUT FOURNIR POUR LE SET (à l'initialisation):

Une fonction de comparaison doit être de la forme suivante:
int function( void* x,  void* y);
- x > y : 1
- x == y : 0
- x < y : -1

Une fonction de copie d'un élément, utilisant un malloc :
exemple simple (pour query replace sur le <type>)
void* <type>_copy( void* x){
  <type> *y = x;
  <type>* y_copy = malloc(sizeof(type));
  *y_copy = *y;
  return y_copy;
}

Une fonction de libération de mémoire, qui libère si besoin des pointeurs situés dans la structure.
void delete(void*);
*/
#endif
