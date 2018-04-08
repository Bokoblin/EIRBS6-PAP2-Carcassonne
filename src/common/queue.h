#ifndef QUEUE_H
#define QUEUE_H

struct queue;

/**
 * @brief Create an empty queue
 * @note Complexity: O(1)
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
 * @return 1 if empty
 * @return 0 otherwise
 */
int queue__is_empty(struct queue *q);

  
/**
 * @brief Push an element in the queue
 * @note Complexity: O(1)
 * @param q the queue
 * @param element the element to add
 * @return 0 on success
 * @return -1 otherwise
 */
int queue__enqueue(struct queue *q, void* element);


/**
 * @brief Retrieve the element on the top of the queue without removing it
 * @note Complexity: O(1)
 * @param q the queue
 * @return an enumeration representing the element on top
 */
void* queue__first(struct queue *q);


/**
 * @brief Pop an element out of the queue
 * @note Complexity: O(1)
 * @param q the queue
 * @return an enumeration representing the popped element
 */
void* queue__dequeue(struct queue *q);


/**
 * @brief Count the number of elements in the queue
 * @note Complexity: O(1)
 * @param q the queue
 * @return an integer corresponding to the number of elements in the queue
 */
unsigned int queue__length(struct queue *q);


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
 */
void queue__debug(struct queue *q);
  
#endif
