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
    enum card_id *array;
    unsigned int capacity;
    unsigned int head;
};


////////////////////////////////////////////////////////////////////
///     STACK FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

struct stack *stack__empty()
{
    struct stack *s = malloc(sizeof(struct stack));
    if (s == NULL) {
        exit_on_error("Malloc failure on: struct stack*");
    } else {
        s->capacity = DEFAULT_STACK_CAPACITY;
        s->head = 0;
        s->array = malloc(sizeof(enum card_id) * (s->capacity));
    }

    return s;
}

int stack__is_empty(struct stack *s)
{
    return (s->head == 0);
}

int stack__push(struct stack *s, enum card_id element)
{
    assert(s->array != NULL);
    if (element > LAST_CARD)
        return -1;

    /* Adjust capacity if necessary */
    if (s->head == s->capacity){
        s->capacity = s->capacity * 2;
        s->array = realloc(s->array, sizeof(enum card_id) * s->capacity);
        assert(s->array != NULL);
    }

    s->array[s->head] = element;
    s->head++;
    return 0;
}

enum card_id stack__peek(struct stack *s)
{
    assert(s->array != NULL);
    if (stack__is_empty(s))
        return LAST_CARD;

    return s->array[s->head-1];
}

enum card_id stack__pop(struct stack *s)
{
    assert(s->array != NULL);
    if (stack__is_empty(s))
        return LAST_CARD;

    // Adjust capacity if necessary
    if ((s->head <= s->capacity / 4) && (s->capacity > DEFAULT_STACK_CAPACITY)) {
        s->capacity = s->capacity / 2;
        s->array = realloc(s->array, sizeof(enum card_id) * s->capacity);
        assert(s->array != NULL);
    }

    s->head--;
    return s->array[s->head];
}


unsigned int stack__length(struct stack *s)
{
    return s->head;
}


void stack__free(struct stack *s)
{
    free(s->array);
    free(s);
}


void stack__debug(struct stack *s)
{
    if (stack__is_empty(s) != 0)
        printf("Pile is empty.");
    else
        for (unsigned int i = 0; i < s->head; i++)
            printf("%d ", s->array[i]);
}
