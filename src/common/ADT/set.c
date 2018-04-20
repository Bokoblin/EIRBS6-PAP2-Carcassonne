#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "set.h"
#include "../utils.h"

#define BASIC_SET_SIZE 8 //must be a power of two

////////////////////////////////////////////////////////////////////
///     STACK STRUCTURE
////////////////////////////////////////////////////////////////////

struct set
{
    void **s;
    size_t capacity;
    size_t size;
    void* (*copy) (void* x);
    int (*cmp) (void* x, void* y);
    void (*delete) (void*);
    void (*debug) (void*);
};

////////////////////////////////////////////////////////////////////
///     USEFUL FUNCTIONS
////////////////////////////////////////////////////////////////////


//dichotomic search
size_t find(struct set const *set, size_t beg, size_t end, void* x)
{
    assert(x != NULL);
    if (beg >= end)
        return beg;

    size_t m = (beg + end)/2;
    if (set->s[m] == x)
        return m;
    if (set->cmp(x, set->s[m]) == 1)
        return find(set, m + 1, end, x);
    else
        return find(set, beg, m, x);
}

void shift_right(struct set const *set, size_t begin, size_t end)
{
    if ((end >= set->capacity - 1) || (end > set->size))
        return;
    //Shift from [begin, end] to [begin + 1, end + 1] by a right to left path
    size_t k = end + 1;
    while (k > begin) {
        set->s[k]=set->s[k-1];
        k--;
    }
}

void shift_left(struct set const *set, size_t begin, size_t end)
{
    if ((begin == 0) || (end != set->size - 1))
        return;

    //Shift from [begin, end] to [begin - 1, end - 1] by a left to right path
    size_t k = begin - 1;
    while (k < end) {
        set->s[k]=set->s[k+1];
        k++;
    }
}

////////////////////////////////////////////////////////////////////
///     SET FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

struct set *set__empty(void *copy_op, void *delete_op, void *compare_op, void *debug_op)
{
    struct set *set = safe_malloc(sizeof(struct set));
    set->capacity = BASIC_SET_SIZE;
    set->s = safe_malloc(sizeof(void*)*set->capacity);
    for (size_t i = 0; i < set->capacity; i++)
        set->s[i] = NULL;
    set->size = 0;
    set->copy = copy_op;
    set->delete = delete_op;
    set->cmp = compare_op;
    set->debug = debug_op;

    return set;
}

int set__is_empty(struct set const *set)
{
    return (set->size == 0);
}

int set__add(struct set *set, void* x)
{
    if  (x == NULL)
        return 1;

    size_t pos = find(set, 0, set->size, x);
    if  ((pos < set->size) && (set->cmp(set->s[pos], x) == 0))
        return 1;

    //Augment memory if needed
    if (set->size == set->capacity) {
        set->capacity = set->capacity * 2;
        set->s = realloc(set->s, sizeof(void*)*set->capacity);
        if (set->s == NULL)
            return -1;
        else
            for (size_t i = set->capacity / 2; i < set->capacity; i++)
                set->s[i] = NULL;
    }

    //Add x into the set
    shift_right(set, pos, set->size);
    set->s[pos] = set->copy(x);
    set->size++;

    return 0;
}

int set__remove(struct set *set, void* x)
{
    assert(set->s != NULL);
    if  (x == NULL)
        return 1;

    size_t pos = find(set, 0, set->size, x);
    if  ((set->size == 0)
            || (pos >= set->size)
            || ((pos < set->size) && (set->cmp(set->s[pos], x) != 0)))
        return 1;

    //Remove the element
    set->delete(set->s[pos]);
    shift_left(set, pos + 1, set->size - 1);
    set->size--;

    //Remove some allocated memory if needed
    if (set->size < set->capacity / 4) {
        set->capacity = set->capacity / 4;
        set->s = realloc(set->s, sizeof(void*)*set->capacity);
        if (set->s == NULL)
            return -1;
    }

    return 0;
}

void* set__retrieve(struct set *set, void* x)
{
    if  (set->s == NULL || x == NULL || set->size == 0)
        return NULL;

    size_t pos = find(set, 0, set->size, x);
    if  (pos >= set->size || (pos < set->size && set->cmp(set->s[pos], x) != 0))
        return NULL;

    return set->s[pos];
}

int set__find(struct set const *set, void* x)
{
    size_t pos = find(set, 0, set->size, x);
    return  ((pos < set->size) && (set->cmp(set->s[pos], x) == 0));
}

size_t set__size(struct set const * set)
{
    return set->size;
}

void* set__get_umpteenth(struct set const *s, size_t i)
{
    if (i >= set__size(s))
        return NULL;
    return s->copy(s->s[i]);
}

void* set__get_umpteenth_no_copy(struct set const *s, size_t i)
{
    if (i >= set__size(s))
        return NULL;
    return s->s[i];
}

void set__free(struct set *set)
{
    for (size_t i = 0; i < set->size; i++)
        set->delete(set->s[i]);
    free(set->s);
    free(set);
}

struct set *set__filter(const struct set *s, int (*filter) (const void*))
{
    struct set *filtered_set = set__empty(s->copy, s->delete, s->cmp, NULL);
    size_t i = 0;
    while ((i < s->capacity) && (i < s->size)) {
        //Augment memory if  needed
        if (filtered_set->size == filtered_set->capacity) {
            filtered_set->capacity = filtered_set->capacity * 2;
            filtered_set->s = realloc(filtered_set->s, sizeof(void*)*filtered_set->capacity);
            if (filtered_set->s == NULL)
                return NULL;
        }
        if (filter(s->s[i])) {
            filtered_set->s[filtered_set->size -1] = s->copy(s->s[i]);
            filtered_set->size++;
        }
        i++;
    }
    return filtered_set;
}

void set__debug_data(const struct set *s)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (s == NULL || s->s == NULL)
        printf("Set (NULL)\n");
    else {
        printf("Set (capacity: %zu, size: %zu, content: \n", s->capacity, s->size);
        printf("\t{ ");
        size_t i = 0;
        while(i < s->size) {
            s->debug(s->s[i]);
            i++;
        }
        printf("}\n)\n");
    }
}
