#ifndef CARDS_H
#define CARDS_H

#include "projCarcassonneInterface.h"
#include "pile.h"


////////////////////////////////////////////////////////////////////
///     COMMON STRUCTURES
////////////////////////////////////////////////////////////////////


/**
 * @brief Area types
 * Lists all the possible area types on a card
 */
enum area_type{
  PATH,
  TOWN,
  FIELD,
  INTERSECTION,
  ABBEY,
};


/**
 * @brief Struct board
 * Points out the first card
 */
struct board{
  struct card* first_card;
  struct card** free_positions;
};


/**
 * @brief Struct card
 * Describes a single card
 */
struct card{
  enum card_id id;
  struct card* north;
  struct card* south;
  struct card* east;
  struct card* west;
  enum area_type type_north_east; // {NE}
  enum area_type type_north; // {N}
  enum area_type type_north_west; // {NW}
  enum area_type type_west; // {W}
  enum area_type type_west_south; // {WS}
  enum area_type type_south_west; // {SW}
  enum area_type type_south; // {S}
  enum area_type type_south_east; // {SE}
  enum area_type type_east_north; // {EN}
  enum area_type type_east; // {E}
  enum area_type type_east_south; // {ES}
  enum area_type type_center; // {C}
};


////////////////////////////////////////////////////////////////////
///     COMMON FUNCTIONS
////////////////////////////////////////////////////////////////////


/** 
 * @brief Initialize a gameboard
 * @return a pointer towards a struct board
 */
struct board* initialize_board();


/** 
 * @brief Initialize a card depending on its identity
 * @param id card ID 
 * @return a pointer towards a struct card
 */
struct card* empty_card(enum card_id id);


/** 
 * @brief places the new card depending next to its neighbors and update their structures 
 * @param id card ID 
 * @return an integer whether it worked or not
 */
int place(struct card* new_card, struct card** neighbor_list);


void free_board(struct board* game);

void free_card(struct card* card);

enum card draw_card(struct pile* p);

/**
 * @brief Rotate the given card in the clockwise turn
 * @param card is the card you want to rotate
 */
void clokwise_rotation(struct card *card);


/**
 * @brief Rotate the given card in the trigonometric turn
 * @param card is the card you want to rotate
 */
void trigo_rotation(struct card *card);

#endif
