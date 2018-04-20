#ifndef STACK_H
#define STACK_H

#include <stddef.h>

/**
 * Implementation of a FILO Abstract Data Type
 *
 * Notes :
 * 1) You have to correctly implement copy, delete and debug operators
 * by handling NULL value, otherwise you can end up with an undefined behaviour.
 * The prototypes of these functions are :
 * void* (*copy_op)(void*)
 * void* (*delete_op)(void*)
 * void* (*debug_op)(void*)
 *
 * 2) stack_peek() and stack_pop() return a dynamically allocated pointer to the head element of
 * the stack in order to make it survive independently of the stack life cycle.
 * The user has to manually free the return pointer after usage.
 */
struct stack;


/**
 * @brief Create an empty stack
 * @note Complexity: O(1)
 * @return a newly created stack
 */
struct stack *stack__empty(void* copy_op, void* delete_op, void* debug_op);


/**
 * @brief Verify if the stack is empty
 * @note Complexity: O(1)
 * @param s the stack
 * @return 1 if empty
 * @return 0 otherwise
 */
int stack__is_empty(struct stack *s);

  
/**
 * @brief Push an element in the stack
 * @note Complexity: O(1)
 * @param s the stack
 * @param element the element to add
 * @return 0 on success, 1 otherwise
 */
int stack__push(struct stack *s, void* element);


/**
 * @brief Retrieve the element on the top of the stack without removing it
 * @note Complexity: O(1)
 * @param s the stack
 * @return an enumeration representing the element on top
 */
void* stack__peek(struct stack *s);


/**
 * @brief Pop an element out of the stack
 * @note Complexity: O(1)
 * @param s the stack
 * @return an enumeration representing the popped element
 */
void* stack__pop(struct stack *s);


/**
 * @brief Count the number of elements in the stack
 * @note Complexity: O(1)
 * @param s the stack
 * @return an integer corresponding to the number of elements in the stack
 */
size_t stack__length(struct stack *s);


/**
 * @brief Randomly mixes the stack's content
 * @note Complexity: O(n)
 * @param s the stack
 * */
void stack__mix(struct stack *s);


/**
 * @brief Free all allocated memory used by the stack
 * @note Complexity: O(n)
 * @param s the stack to free
 */
void stack__free(struct stack *s);


/**
 * @brief Prints the stack's content
 * @note Complexity: O(n)
 * @param s the stack to debug
 * @param is_compact to display a compact debug (only values)
 */
void stack__debug(struct stack *s, int is_compact);
  
#endif
