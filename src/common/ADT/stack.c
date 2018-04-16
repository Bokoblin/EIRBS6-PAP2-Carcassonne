#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"
#include "../utils.h"

#define DEFAULT_STACK_CAPACITY 2


int my_random(int min, int max){ //min included, max exlcuded
  return min + (rand() % (max -  min));
}

////////////////////////////////////////////////////////////////////
///     STACK STRUCTURE
////////////////////////////////////////////////////////////////////

struct stack
{
    void* *array;
    unsigned int capacity;
    unsigned int head;
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
    if (s == NULL)
        return -1;

    return (s->head == 0);
}

int stack__push(struct stack *s, void* element)
{
    if (s == NULL || s->array == NULL || element == NULL)
        return -1;

    /* Adjust capacity if necessary */
    if (s->head == s->capacity){
        s->capacity = s->capacity * 2;
        s->array = realloc(s->array, sizeof(void*) * s->capacity);
        if (s->array == NULL)
            return -1;
    }

    s->array[s->head] = s->operator_copy(element);
    s->head++;
    return 0;
}

void* stack__peek(struct stack *s)
{
    if (s == NULL || s->array == NULL || stack__is_empty(s))
        return NULL;

    return s->operator_copy(s->array[s->head-1]);
}

void* stack__pop(struct stack *s)
{
    if (s == NULL || s->array == NULL || stack__is_empty(s))
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
    if (s == NULL)
        return 0;

    return s->head;
}

void stack__mix(struct stack *s)
{
    if (s == NULL || s->array == NULL)
        return;

    size_t a;
    size_t b;
    void* tmp;  
    for (unsigned int i = 0; i < s->head; i++){
        a = my_random(0, s->head);
        b = my_random(0, s->head);
        tmp = s->array[a];
        s->array[a] = s->array[b];
        s->array[b] = tmp;
    }
}

void stack__free(struct stack *s)
{
    if (s == NULL || s->array == NULL)
        return;

    for (unsigned int i = 0; i < s->head; i++){
        if (s->array[i] != NULL)
            s->operator_delete(s->array[i]);
    }
    free(s->array);
    free(s);
}

void stack__debug(struct stack *s)
{
    if (s == NULL || s->array == NULL)
        return;

    if (stack__is_empty(s) != 0)
        printf("Stack is empty.");
    else
        for (unsigned int i = 0; i < s->head; i++)
            s->operator_debug(s->array[i]);
}
