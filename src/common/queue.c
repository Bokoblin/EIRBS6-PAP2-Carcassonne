#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "utils.h"

#define DEFAULT_QUEUE_CAPACITY 2

////////////////////////////////////////////////////////////////////
///     QUEUE STRUCTURE
////////////////////////////////////////////////////////////////////

struct queue
{
    void **array;
    unsigned int capacity;
    unsigned int size;
    void* (*operator_copy) (void*);
    void (*operator_delete)(void *);
    void (*operator_debug)(void *);
};


////////////////////////////////////////////////////////////////////
///     QUEUE FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

struct queue *queue__empty(void* copy_op, void* delete_op, void* debug_op)
{
    struct queue *q = malloc(sizeof(struct queue));
    if (q == NULL) {
        exit_on_error("Malloc failure on: struct queue*");
    } else {
        q->array = malloc(sizeof(void *) * DEFAULT_QUEUE_CAPACITY);
        if (q->array == NULL) {
            exit_on_error("Malloc failure on: void**");

            q->capacity = DEFAULT_QUEUE_CAPACITY;
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
        q->array = realloc(q->array, sizeof(void *) * q->capacity);
        if (q->array == NULL)
            exit_on_error("Realloc failure on: void**");

        q->capacity = q->capacity * 2;
    }

    q->array[q->size - 1] = q->operator_copy(element);
    q->size++;

    return 0;
}

void* queue__first(struct queue *q)
{
    if (q == NULL || q->array == NULL)
        return NULL;

    if (queue__is_empty(q))
        return NULL;

    return q->array[0];
}

void* queue__dequeue(struct queue *q)
{
    if (q == NULL || q->array == NULL)
        return NULL;

    if (queue__is_empty(q))
        return NULL;

    void  *dequeued_e = q->array[0];

    if (q->size > 1) {
        for (unsigned int i = 0; i < q->size-1; i++) {
            q->operator_delete(q->array[i]);
            q->array[i] = q->operator_copy(q->array[i+1]);
        }
    }

    q->size--;
    return dequeued_e;
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

    free(q->array);
    free(q);
}

void queue__debug(struct queue *q)
{
    if (q == NULL || q->array == NULL)
        return;

    if (queue__is_empty(q) != 0)
        printf("Queue is empty.");
    else
        for (unsigned int i = 0; i < q->size; i++)
            q->operator_debug(q->array[i]);
}
