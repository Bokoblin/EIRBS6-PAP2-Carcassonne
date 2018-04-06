#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"
#include "utils.h"

#define DEFAULT_STACK_CAPACITY 2

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

struct stack *stack__empty(void* (*copy) (void*),
                           void (*my_delete) (void*),
                           void (*debug) (void*))
{
    struct stack *s = malloc(sizeof(struct stack));
    if (s == NULL) {
        exit_on_error("Malloc failure on: struct stack*");
    } else {
        s->capacity = DEFAULT_STACK_CAPACITY;
        s->head = 0;
        s->array = malloc(sizeof(void*) * (s->capacity));
        s->operator_copy = copy;
        s->operator_delete = my_delete;
        s->operator_debug = debug;
    }
    return s;
}

int stack__is_empty(struct stack *s)
{
    return (s->head == 0);
}

int stack__push(struct stack *s, void* element)
{
    assert(s->array != NULL);
    if (element == NULL)
        return -1;

    /* Adjust capacity if necessary */
    if (s->head == s->capacity){
        s->capacity = s->capacity * 2;
        s->array = realloc(s->array, sizeof(void*) * s->capacity);
        assert(s->array != NULL);
    }

    s->array[s->head] = s->operator_copy(element);
    s->head++;
    return 0;
}

void* stack__peek(struct stack *s)
{
    assert(s->array != NULL);
    if (stack__is_empty(s))
        return NULL;

    return s->operator_copy(s->array[s->head-1]);
}

void* stack__pop(struct stack *s)
{
    assert(s->array != NULL);
    if (stack__is_empty(s))
        return NULL;

    // Adjust capacity if necessary
    if ((s->head <= s->capacity / 4) && (s->capacity > DEFAULT_STACK_CAPACITY)) {
        s->capacity = s->capacity / 2;
        s->array = realloc(s->array, sizeof(void*) * s->capacity);
        assert(s->array != NULL);
    }

    s->head--;
    void* returned = s->operator_copy(s->array[s->head]);
    s->operator_delete(s->array[s->head]);
    s->array[s->head] = NULL;
    return returned;
}


unsigned int stack__length(struct stack *s)
{
    return s->head;
}


void stack__free(struct stack *s)
{
    for (unsigned int i = 0; i < s->head; i++){
        if (s->array[i] != NULL)
            s->operator_delete(s->array[i]);
    }
    free(s->array);
    free(s);
}


void stack__debug(struct stack *s)
{
    if (stack__is_empty(s) != 0)
        printf("Stack is empty.");
    else
        for (unsigned int i = 0; i < s->head; i++)
            s->operator_debug(s->array[i]);
}
