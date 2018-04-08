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
    size_t capacity;
    size_t top;
    size_t size;
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
        if (q->array == NULL)
            exit_on_error("Malloc failure on: void**");
        else
            for (size_t i = 0; i < DEFAULT_QUEUE_CAPACITY; i++)
                q->array[i] = NULL;

        q->capacity = DEFAULT_QUEUE_CAPACITY;
        q->top = 0;
        q->size = 0;
        q->operator_copy = copy_op;
        q->operator_delete = delete_op;
        q->operator_debug = debug_op;
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

    // Adjust capacity if necessary
    if (q->size == q->capacity) {
        q->array = realloc(q->array, sizeof(void *) * q->capacity * 2);
        if (q->array == NULL)
            exit_on_error("Realloc failure on: void**");
        else
            q->capacity = q->capacity * 2;

        // Copy of the queue in the new allocated memory to avoid modulo issue
        size_t new_top = q->capacity / 2;
        size_t i = q->top;
        size_t j = new_top;
        while (positive_modulo((int) (q->top + q->size), (int) new_top) != i) {
            q->array[j] = q->array[i];
            i = positive_modulo(i, (int) new_top);
            i++;
            j++;
        }
        q->top = new_top;
    }

    q->array[positive_modulo((int) (q->size + q->top), (int) q->capacity)] = q->operator_copy(element);
    q->size++;

    return 0;
}
void* queue__dequeue(struct queue *q)
{
    if (q == NULL || q->array == NULL || queue__is_empty(q))
        return NULL;


    //TODO : free some space if the queue size > queue capacity

    void *dequeued_e = q->operator_copy(q->array[positive_modulo((int) q->top, (int) q->capacity)]);
    q->top = positive_modulo((int) (q->top + 1), (int) q->capacity);
    q->size--;

    q->operator_delete(q->array[positive_modulo((int) q->top, (int) q->capacity)]);
    q->array[positive_modulo((int) q->top, (int) q->capacity)] = NULL;

    return dequeued_e;
}

void* queue__front(struct queue *q)
{
    if (q == NULL || q->array == NULL || queue__is_empty(q))
        return NULL;

    return q->array[q->top];
}

void* queue__back(struct queue *q)
{
    if (q == NULL || q->array == NULL || queue__is_empty(q))
        return NULL;

    return q->array[positive_modulo((int) (q->size + q->top), (int) q->capacity)];
}

size_t queue__length(struct queue *q)
{
    if (q == NULL || q->array == NULL)
        return 0;

    return q->size;
}

void queue__free(struct queue *q)
{
    if (q == NULL || q->array == NULL)
        return;

    size_t i = q->top;
    while (positive_modulo((int) (q->top + q->size), (int) q->capacity) != i) {
        q->operator_delete(q->array[i]);
        i = positive_modulo(i, (int) q->capacity);
        i++;
    }
    free(q->array);
    free(q);
}

void queue__debug(struct queue *q)
{
    if (q == NULL || q->array == NULL)
        return;

    if (queue__is_empty(q) != 0)
        printf("Queue is empty.\n");
    else {
        size_t i = q->top;
        while (positive_modulo((int) (q->top + q->size), (int) q->capacity) != i) {
            q->operator_debug(q->array[i]);
            i = positive_modulo(i, (int) q->capacity);
            i++;
        }
    }
}
