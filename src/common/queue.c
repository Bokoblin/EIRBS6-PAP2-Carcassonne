#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "utils.h"

#define DEFAULT_QUEUE_CAPACITY 2 //Must be a power of 2

////////////////////////////////////////////////////////////////////
///     QUEUE STRUCTURE
////////////////////////////////////////////////////////////////////

struct queue
{
    void **array;
    unsigned int capacity;
    unsigned int top;
    unsigned int size;
    void* (*operator_copy) (void*);
    void (*operator_delete)(void *);
    void (*operator_debug)(void *);
};


////////////////////////////////////////////////////////////////////
///     QUEUE FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////


/* Required because:
-1 % 50 != 49
modulo(-1, 50) = 49
*/
unsigned int modulo(int a, int b){
  if (b <= 0)
    return 0;
  if (a >= 0)
    return a%b;
  while (a < 0){
    a = a + b;
  }
  return a%b;
}

struct queue *queue__empty(void* copy_op, void* delete_op, void* debug_op)
{
    struct queue *q = malloc(sizeof(struct queue));
    if (q == NULL) {
        exit_on_error("Malloc failure on: struct queue*");
    } else {
        if (q->array == NULL) {
            exit_on_error("Malloc failure on: void**");

            q->capacity = DEFAULT_QUEUE_CAPACITY;
            q->array = malloc(sizeof(void *) * q->capacity);
            q->top = 0;
            q->size = 0;
            q->operator_copy = copy_op;
            q->operator_delete = delete_op;
            q->operator_debug = debug_op;
        }
    }

    return q;
}

int queue__is_empty(struct queue *q)
{
    if (q == NULL || q->array == NULL)
        return 1;

    return q->size == 0;
}

int queue__enqueue(struct queue *q, void* element)
{
    if (q == NULL || q->array == NULL || element == NULL)
        return -1;

    /* Adjust capacity if necessary */
    if (q->size == q->capacity) {
      q->capacity = q->capacity * 2;
        printf("%d\n",  q->capacity);
        q->array = realloc(q->array, sizeof(void *) * q->capacity);
        if (q->array == NULL)
            exit_on_error("Realloc failure on: void**");

        /* Copy of the queue in the new allocated memory to avoid modulo issue */
        unsigned int new_top = q->capacity / 2;
        unsigned int i = q->top;
        unsigned int j = new_top;
        while(modulo(q->top + q->size, new_top) != i){
            q->array[j] = q->array[i];
            j++;
            i = modulo(i++, new_top);
        }
        q->top = new_top;
    }

    q->array[modulo((q->size + q->top), q->capacity)] = q->operator_copy(element);
    q->size++;

    return 0;
}
void* queue__dequeue(struct queue *q)
{
    if (q == NULL || q->array == NULL || queue__is_empty(q))
        return NULL;


    //TODO : free some space if the queue size > queue capacity
    
    void *dequeued_e = q->operator_copy(q->array[modulo((q->top), q->capacity)]);
    q->top = modulo((q->top + 1), q->capacity);
    q->size--;
    
    q->operator_delete(q->array[modulo((q->top), q->capacity)]);
    q->array[modulo((q->top), q->capacity)] = NULL;

    return dequeued_e;
}

void* queue__first(struct queue *q)
{
    if (q == NULL || q->array == NULL || queue__is_empty(q))
        return NULL;

    return q->array[q->top];
}

void* queue__last(struct queue *q)
{
    if (q == NULL || q->array == NULL || queue__is_empty(q))
        return NULL;

    return q->array[modulo((q->size + q->top), q->capacity)];
}

unsigned int queue__length(struct queue *q)
{
    if (q == NULL || q->array == NULL)
        return 0;

    return q->size;
}

void queue__free(struct queue *q)
{
    if (q == NULL || q->array == NULL)
        return;

    unsigned int i = q->top;
    while(modulo(q->top + q->size, q->capacity) != i){
        q->operator_delete(q->array[i]);
        i = modulo(i++, q->capacity);
    }
    free(q->array);
    free(q);
}

void queue__debug(struct queue *q)
{
    if (q == NULL || q->array == NULL)
        return;

    if (queue__is_empty(q) != 0)
        printf("Queue is empty. ");
    else{
        unsigned int i = q->top;
        while(modulo(q->top + q->size, q->capacity) != i){
            q->operator_debug(q->array[i]);
            i = modulo(i++, q->capacity);
        }
    }
}
