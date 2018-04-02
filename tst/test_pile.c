#include <stdlib.h>
#include <stdio.h>
#include "../src/common/pile.h"

/* ALL TESTS AVAILABLE FOR THE ENUM CARD_ID PILE*/

int test_empty_pile(){
  struct pile *p = pile__empty();
  
  if (p == NULL){
    pile__free(p);
    return 1;
  }
  pile__free(p);
  return 0;
}


int test_is_empty_empty_pile(){
  struct pile *p = pile__empty();
  
  if (!pile__is_empty(p)){
    pile__free(p);
    return 1;
  }
  pile__free(p);
  return 0;
}


int test_is_empty_non_empty_pile(){
  struct pile *p = pile__empty();
  enum card_id card = 1;
  pile__stack(p, card);
  
  if (pile__is_empty(p)){
    pile__free(p);
    return 1;
  }
  pile__free(p);
  return 0;
}


int test_stack_empty_pile(){ //Same as above actually
  struct pile *p = pile__empty();
  enum card_id card = 1;
  pile__stack(p, card);
  
  if (pile__is_empty(p)){
    pile__free(p);
    return 1;
  }
  pile__free(p);
  return 0;
}


int test_stack_non_empty_pile(){
  struct pile *p = pile__empty();
  enum card_id card1 = 1;
  enum card_id card2 = 2;
  enum card_id card3 = 3;
  enum card_id card4 = 4;
  pile__stack(p, card1);
  pile__stack(p, card2);
  pile__stack(p, card3);
  pile__stack(p, card4);
  pile__stack(p, card1);
  
  if (pile__length(p) != 5){
    pile__free(p);
    return 1;
  }
  pile__free(p);
  return 0;
}


int test_stack_NULL(){
  struct pile *p = pile__empty();
  enum card_id card = 999;  
  pile__stack(p, card);
  
  if (!pile__is_empty(p)){
    pile__free(p);
    return 1;
  }
  pile__free(p);
  return 0;
}


int test_stack_a_lot(){
  struct pile *p = pile__empty();
  enum card_id card = 1;
  for (unsigned int i = 0; i < 500; i++){
    pile__stack(p, card);
  }

  if (pile__length(p) < 500){
    pile__free(p);
    return 1;
  }
  pile__free(p);
  return 0;
}


int test_unstack_empty_pile(){
  struct pile *p = pile__empty();
  enum card_id unstacked = pile__unstack(p);
  enum card_id last_card = LAST_CARD;
  
  if (last_card != unstacked){
    pile__free(p);
    return 1;
  }
  pile__free(p);
  return 0;
}


int test_unstack_non_empty_pile(){
  struct pile *p = pile__empty();
  enum card_id card1 = 1;
  enum card_id card2 = 2;
  pile__stack(p, card1);
  pile__stack(p, card2);
  enum card_id unstacked2 = pile__unstack(p);
  enum card_id unstacked1 = pile__unstack(p);

  
  if (!pile__is_empty(p) || (unstacked1 != 1) || (unstacked2 != 2)){
    pile__free(p);
    return 1;
  }
  pile__free(p);
  return 0;
}


int test_pile_length(){
  struct pile *p = pile__empty();
  enum card_id card1 = 1;
  enum card_id card2 = 2;
  pile__stack(p, card1);
  if (pile__length(p) != 1){
    pile__free(p);
    return 1;
  }
  
  pile__stack(p, card2);
  if (pile__length(p) != 2){
    pile__free(p);
    return 1;
  }
  pile__free(p);
  return 0;
}

int test_pile_debug(){
  struct pile *p = pile__empty();
  enum card_id card1 = 1;
  enum card_id card2 = 2;
  enum card_id card3 = 3;
  enum card_id card4 = 4;
  pile__stack(p, card1);
  pile__stack(p, card2);
  pile__stack(p, card3);
  pile__stack(p, card4);
  pile__stack(p, card1);

  pile__debug(p);
  pile__free(p);
  return 0;
}


/* Prints if a test succeeded or not, in color */
void print_success(int result){
  if(result)
    printf("\x1B[31mFAILED\e[0m\n");
  else
    printf("\x1B[32mSUCCESS\e[0m\n");
}

int main(){
  printf("----------- TEST PILE -----------\n");

  
  printf("test_empty_pile ");
  print_success(test_empty_pile());

  printf("test_is_empty_empty_pile ");
  print_success(test_is_empty_empty_pile());
  
  printf("test_is_empty_non_empty_pile ");
  print_success(test_is_empty_non_empty_pile());

  printf("test_stack_empty_pile ");
  print_success(test_stack_empty_pile());

  printf("test_stack_non_empty_pile ");
  print_success(test_stack_non_empty_pile());

  printf("test_stack_NULL ");
  print_success(test_stack_NULL());

  printf("test_stack_a_lot ");
  print_success(test_stack_a_lot());

  printf("test_unstack_empty_pile ");
  print_success(test_unstack_empty_pile());

  printf("test_unstack_non_empty_pile ");
  print_success(test_unstack_non_empty_pile());

  printf("test_pile_length ");
  print_success(test_pile_length());

  printf("test_pile_debug (expected 1 2 3 4 1) ");
  print_success(test_pile_debug());
  
  return 0;
}  

