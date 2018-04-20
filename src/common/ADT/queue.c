#include "queue.h"
#include "../utils.h"

#define DEFAULT_QUEUE_CAPACITY 2 //Must be a power of 2

////////////////////////////////////////////////////////////////////
///     QUEUE STRUCTURE
////////////////////////////////////////////////////////////////////

struct queue
{
    void **array;
    size_t capacity;
    size_t front;
    size_t back;
    void* (*operator_copy) (void*);
    void (*operator_delete)(void *);
    void (*operator_debug)(void *);
};


////////////////////////////////////////////////////////////////////
///     QUEUE FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

struct queue *queue__empty(void* copy_op, void* delete_op, void* debug_op)
{
    struct queue *q = safe_malloc(sizeof(struct queue));

    q->array = safe_malloc(sizeof(void *) * DEFAULT_QUEUE_CAPACITY);
    for (size_t i = 0; i < DEFAULT_QUEUE_CAPACITY; i++)
        q->array[i] = NULL;

    q->capacity = DEFAULT_QUEUE_CAPACITY;
    q->front = 0;
    q->back = 0;
    q->operator_copy = copy_op;
    q->operator_delete = delete_op;
    q->operator_debug = debug_op;

    return q;
}

int queue__is_empty(struct queue *q)
{
    assert_not_null(q, __func__, "queue parameter");

    return queue__length(q) == 0;
}

int queue__enqueue(struct queue *q, void* element)
{
    assert_not_null(q, __func__, "queue parameter");

    if (element == NULL)
        return !SUCCESS;

    // Adjust capacity if necessary
    if (queue__length(q) == q->capacity) {
        size_t prev_capacity = q->capacity;
        q->capacity *= 2;
        q->array = realloc(q->array, sizeof(void *) * q->capacity);
        if (q->array == NULL)
            return !SUCCESS;
        else
            for (size_t i = prev_capacity; i < q->capacity; i++)
                q->array[i] = NULL;

        q->back = prev_capacity;
        for (size_t i = 0; i < q->front; i++) {
            q->array[q->back] = q->array[i];
            q->array[0] = NULL;
            q->back++;
        }
    }

    q->array[q->back] = q->operator_copy(element);
    q->back = (q->back + 1) % q->capacity;

    return SUCCESS;
}

int queue__dequeue(struct queue *q)
{
    assert_not_null(q, __func__, "queue parameter");

    if (queue__is_empty(q))
        return !SUCCESS;

    q->operator_delete(q->array[q->front]);
    q->array[q->front] = NULL;
    q->front = (q->front + 1) %q->capacity;

    if (queue__length(q) < q->capacity / 2 && q->capacity > 2) {
        size_t new_pos = 0;
        for (size_t i = q->front; i != q->back; i = (i+1) % q->capacity ) {
            q->array[new_pos] = q->array[i];
            q->array[i] = NULL;
            new_pos++;
        }
        q->front = 0;
        q->back = positive_modulo((int) (new_pos), (int) q->capacity);

        q->array = realloc(q->array, sizeof(void *) * q->capacity / 2);
        if (q->array == NULL)
            return !SUCCESS;
        else
            q->capacity /= 2;
    }

    return SUCCESS;
}

void* queue__front(struct queue *q)
{
    assert_not_null(q, __func__, "queue parameter");

    if (queue__is_empty(q))
        return NULL;

    return q->operator_copy(q->array[q->front]);
}

void* queue__back(struct queue *q)
{
    assert_not_null(q, __func__, "queue parameter");

    if (queue__is_empty(q))
        return NULL;

    if (q->back == 0)
        return q->operator_copy(q->array[q->capacity - 1]);
    else
        return q->operator_copy(q->array[q->back-1]);
}

size_t queue__length(struct queue *q)
{
    assert_not_null(q, __func__, "queue parameter");

    if (q->back > q->front || q->array[q->front] == NULL)
        return q->back - q->front;
    else
        return q->back + q->capacity - q->front;
}

void queue__free(struct queue *q)
{
    if (q == NULL)
        return;

    for (size_t i = 0; i < q->capacity; i++)
        if (q->array[i] != NULL)
            q->operator_delete(q->array[i]);

    free(q->array);
    free(q);
}

void queue__debug(struct queue *q, int is_compact)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (q == NULL || q->array == NULL)
        printf("Queue (NULL)\n");
    else {
        if (!is_compact) {
            printf("Queue (capacity: %zu, size: %zu, content: \n", q->capacity, queue__length(q));
            printf("\t in queue order: { ");
            for (size_t i = q->front; i != q->back; i = (i+1) % q->capacity ) {
                q->operator_debug(q->array[i]);
            }
            printf("}\n\t in array order: { ");
            for (size_t i = 0; i < q->capacity; i++) {
                q->operator_debug(q->array[i]);
            }
            printf("}\n)\n");
        } else {
            printf("{ ");
            for (size_t i = q->front; i != q->back; i = (i+1) % q->capacity ) {
                q->operator_debug(q->array[i]);
            }
            printf("}");
        }
    }
}
