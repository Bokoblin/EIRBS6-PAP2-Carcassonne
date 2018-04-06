#ifndef STACK_H
#define STACK_H

#include "common_interface.h"

struct stack;

/**
 * @brief Create an empty stack
 * @note Complexity: O(1)
 * @return a newly created stack
 */
struct stack *stack__empty(void* (*operator_copy) (void*),
        void (*operator_delete) (void*),
        void (*operator_debug) (void*));


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
 * @return 0 on success
 * @return -1 otherwise
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
unsigned int stack__length(struct stack *s);


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
 */
void stack__debug(struct stack *s);
  
#endif
