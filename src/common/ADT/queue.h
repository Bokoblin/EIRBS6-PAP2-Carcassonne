#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

/**
 * Implementation of a FIFO Abstract Data Type
 *
 * Notes :
 * 1) You have to correctly implement copy, delete and debug operators
 * by handling NULL value, otherwise you can end up with an undefined behaviour.
 * The prototypes of these functions are :
 * void* (*copy_op)(void*)
 * void* (*delete_op)(void*)
 * void* (*debug_op)(void*)
 *
 * 2) queue__front() and queue_back() return a dynamically allocated pointer to an element of
 * the queue in order to make it survive independently of the queue life cycle.
 * The user has to manually free the return pointer after usage.
 */
struct queue;


/**
 * @brief Create an empty queue
 * @note Complexity: O(n)
 * @param copy_op copy operator
 * @param delete_op delete operator
 * @param debug_op debug operator
 * @return a newly created queue
 */
struct queue *queue__empty(void* copy_op, void* delete_op, void* debug_op);


/**
 * @brief Verify if the queue is empty
 * @note Complexity: O(1)
 * @param q the queue
 * @return 1 if empty, 0 otherwise
 */
int queue__is_empty(struct queue *q);

  
/**
 * @brief Push an element in the queue
 * @note Complexity: O(n)
 * @param q the queue
 * @param element the element to add
 * @return 0 on success, 1 otherwise
 */
int queue__enqueue(struct queue *q, void* element);


/**
 * @brief Retrieve a copy of the front element
 * @details The element must be manually freed by user afterward
 * @note Complexity: O(1)
 * @param q the queue
 * @return a dynamically allocated copy of the front element pointer
 */
void* queue__front(struct queue *q);


/**
 * @brief Retrieve a copy of the back element
 * @details The element must be manually freed by user afterward
 * @note Complexity: O(1)
 * @param q the queue
 * @return a dynamically allocated copy of the back element pointer
 */
void* queue__back(struct queue *q);


/**
 * @brief Pop the front element out of the queue
 * @note Complexity: O(n)
 * @param q the queue
 * @return 0 on success, 1 otherwise
 */
int queue__dequeue(struct queue *q);


/**
 * @brief Count the number of elements in the queue
 * @note Complexity: O(1)
 * @param q the queue
 * @return an integer corresponding to the number of elements in the queue
 */
size_t queue__length(struct queue *q);


/**
 * @brief Free all allocated memory used by the queue
 * @note Complexity: O(n)
 * @param q the queue to free
 */
void queue__free(struct queue *q);


/**
 * @brief Prints the queue's content
 * @note Complexity: O(n)
 * @param q the queue to debug
 * @param is_compact to display a compact debug (only values)
 */
void queue__debug(struct queue *q, int is_compact);
  
#endif
