#include "stack.h"
#include "../utils.h"

#define DEFAULT_STACK_CAPACITY 2


int my_random(int min, int max){ //min included, max excluded
  return min + (rand() % (max -  min));
}

////////////////////////////////////////////////////////////////////
///     STACK STRUCTURE
////////////////////////////////////////////////////////////////////

struct stack
{
    void* *array;
    size_t capacity;
    size_t head;
    void* (*operator_copy) (void*);
    void (*operator_delete) (void*);
    void (*operator_debug) (void*);
};


////////////////////////////////////////////////////////////////////
///     STACK FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

struct stack *stack__empty(void* copy_op, void* delete_op, void* debug_op)
{
    struct stack *s = safe_malloc(sizeof(struct stack));

    s->capacity = DEFAULT_STACK_CAPACITY;
    s->head = 0;
    s->array = safe_malloc(sizeof(void*) * (s->capacity));
    s->operator_copy = copy_op;
    s->operator_delete = delete_op;
    s->operator_debug = debug_op;

    return s;
}

int stack__is_empty(struct stack *s)
{
    assert_not_null(s, __func__, "stack parameter");
    return (s->head == 0);
}

int stack__push(struct stack *s, void* element)
{
    assert_not_null(s, __func__, "stack parameter");
    assert_not_null(s->array, __func__, "stack array");

    if (element == NULL)
        return !SUCCESS;

    /* Adjust capacity if necessary */
    if (s->head == s->capacity){
        s->capacity = s->capacity * 2;
        s->array = realloc(s->array, sizeof(void*) * s->capacity);
        if (s->array == NULL)
            return !SUCCESS;
    }

    s->array[s->head] = s->operator_copy(element);
    s->head++;

    return SUCCESS;
}

void* stack__peek(struct stack *s)
{
    assert_not_null(s, __func__, "stack parameter");
    assert_not_null(s->array, __func__, "stack array");

    if (stack__is_empty(s))
        return NULL;

    return s->operator_copy(s->array[s->head-1]);
}

void* stack__pop(struct stack *s)
{
    assert_not_null(s, __func__, "stack parameter");
    assert_not_null(s->array, __func__, "stack array");

    if (stack__is_empty(s))
        return NULL;

    // Adjust capacity if necessary
    if ((s->head <= s->capacity / 4) && (s->capacity > DEFAULT_STACK_CAPACITY)) {
        s->capacity = s->capacity / 2;
        s->array = realloc(s->array, sizeof(void*) * s->capacity);
        if (s->array == NULL)
            return NULL;
    }

    s->head--;
    void* returned = s->operator_copy(s->array[s->head]);
    s->operator_delete(s->array[s->head]);
    s->array[s->head] = NULL;
    return returned;
}

size_t stack__length(struct stack *s)
{
    assert_not_null(s, __func__, "stack parameter");
    return s->head;
}

void stack__mix(struct stack *s)
{
    assert_not_null(s, __func__, "stack parameter");
    assert_not_null(s->array, __func__, "stack array");

    size_t a;
    size_t b;
    void* tmp;  
    for (size_t i = 0; i < s->head; i++){
        a = (size_t) my_random(0, (int) s->head);
        b = (size_t) my_random(0, (int) s->head);
        tmp = s->array[a];
        s->array[a] = s->array[b];
        s->array[b] = tmp;
    }
}

void stack__free(struct stack *s)
{
    if (s == NULL || s->array == NULL)
        return;

    for (size_t i = 0; i < s->head; i++){
        if (s->array[i] != NULL)
            s->operator_delete(s->array[i]);
    }
    free(s->array);
    free(s);
}

void stack__debug(struct stack *s, int is_compact)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (s == NULL || s->array == NULL)
        printf("Queue (NULL)\n");
    else {
        if (!is_compact)
            printf("Stack (capacity: %zu, size: %zu, content: \n\t", s->capacity, stack__length(s));
        printf("{ ");
        for (size_t i = 0; i < s->head; i++)
            s->operator_debug(s->array[i]);
        printf("} ");
        if (!is_compact)
            printf("\n)\n");
    }
}
