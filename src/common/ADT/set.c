#include "set.h"
#include "../utils.h"

#define BASIC_SET_SIZE 8 //must be a power of two

////////////////////////////////////////////////////////////////////
///     SET STRUCTURE
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


/**
 * Search an element recursively with dichotomous technique
 * @param set the set
 * @param begin the first index
 * @param end the last index
 * @param searched_e the searched element (criteria given in compare function pointer)
 * @return the index of the found value or end pointer
 */
size_t find(struct set const *set, size_t begin, size_t end, void* searched_e)
{
    if (searched_e == NULL)
        return end;

    if (begin >= end)
        return end;

    size_t middle_e = (begin + end)/2;

    int cmp_res = set->cmp(searched_e, set->s[middle_e]);

    if (cmp_res == 0)
        return middle_e;
    if (cmp_res == 1)
        return find(set, middle_e + 1, end, searched_e);
    else
        return find(set, begin, middle_e, searched_e);
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
    assert_not_null(set, __func__, "set parameter");

    return (set->size == 0);
}

int set__add(struct set *set, void* x)
{
    assert_not_null(set, __func__, "set parameter");
    assert_not_null(set->s, __func__, "set array");

    if  (x == NULL)
        return !SUCCESS;

    size_t pos = find(set, 0, set->size, x);
    if  ((pos < set->size) && (set->cmp(set->s[pos], x) == 0))
        return !SUCCESS;

    //Increase memory if needed
    if (set->size == set->capacity - 1) {
        set->capacity = set->capacity * 2;
        set->s = realloc(set->s, sizeof(void *) * set->capacity);
        if (set->s == NULL)
            return !SUCCESS;
        else
            for (size_t i = set->capacity / 2; i < set->capacity; i++)
                set->s[i] = NULL;
    }

    //Add x into the set
    shift_right(set, pos, set->size);
    set->s[pos] = set->copy(x);
    set->size++;

    return SUCCESS;
}

int set__add_no_copy(struct set *s, void* e)
{
    assert_not_null(s, __func__, "s parameter");
    assert_not_null(s->s, __func__, "s array");

    if  (e == NULL)
        return !SUCCESS;

    size_t pos = find(s, 0, s->size, e);
    if  ((pos < s->size) && (s->cmp(s->s[pos], e) == 0))
        return !SUCCESS;

    //Increase memory if needed
    if (s->size == s->capacity - 1) {
        s->capacity = s->capacity * 2;
        s->s = realloc(s->s, sizeof(void *) * s->capacity);
        if (s->s == NULL)
            return !SUCCESS;
        else
            for (size_t i = s->capacity / 2; i < s->capacity; i++)
                s->s[i] = NULL;
    }

    //Add e into the s
    shift_right(s, pos, s->size);
    s->s[pos] = e;
    s->size++;

    return SUCCESS;
}

int set__remove(struct set *set, void* x)
{
    assert_not_null(set, __func__, "set parameter");
    assert_not_null(set->s, __func__, "set array");

    if  (x == NULL)
        return !SUCCESS;

    size_t pos = find(set, 0, set->size, x);
    if  ((set->size == 0)
            || (pos >= set->size)
            || ((pos < set->size) && (set->cmp(set->s[pos], x) != 0)))
        return !SUCCESS;

    //Remove the element
    set->delete(set->s[pos]);
    shift_left(set, pos + 1, set->size - 1);
    set->size--;

    //Remove some allocated memory if needed
    if (set->size < set->capacity / 4) {
        set->capacity = set->capacity / 4;
        set->s = realloc(set->s, sizeof(void*)*set->capacity);
        if (set->s == NULL)
            return !SUCCESS;
    }

    return SUCCESS;
}

void* set__retrieve(struct set *set, void* x)
{
    assert_not_null(set, __func__, "set parameter");
    assert_not_null(set->s, __func__, "set array");

    if  (x == NULL || set->size == 0)
        return NULL;

    size_t pos = find(set, 0, set->size, x);
    if (pos >= set->size || (pos < set->size && set->cmp(set->s[pos], x) != 0))
        return NULL;

    return set->s[pos];
}

int set__find(struct set const *set, void* x)
{
    assert_not_null(set, __func__, "set parameter");
    assert_not_null(set->s, __func__, "set array");

    size_t pos = find(set, 0, set->size, x);
    return pos < set->size && set->cmp(set->s[pos], x) == 0;
}

size_t set__size(struct set const * set)
{
    assert_not_null(set, __func__, "set parameter");
    assert_not_null(set->s, __func__, "set array");

    return set->size;
}

void* set__get_umpteenth(struct set const *s, size_t i)
{
    assert_not_null(s, __func__, "s parameter");
    assert_not_null(s->s, __func__, "s array");
    if (i >= set__size(s))
        return NULL;
    return s->copy(s->s[i]);
}

void* set__get_umpteenth_no_copy(struct set const *s, size_t i)
{
    assert_not_null(s, __func__, "s parameter");
    assert_not_null(s->s, __func__, "s array");

    if (i >= set__size(s))
        return NULL;
    return s->s[i];
}


struct set *set__fusion(struct set const *set1, struct set const *set2)
{
    assert_not_null(set1, __func__, "set1 parameter");
    assert_not_null(set1->s, __func__, "set1 array");
    assert_not_null(set2, __func__, "set2 parameter");
    assert_not_null(set2->s, __func__, "set2 array");

    if (((void*) set1->copy   != (void*) set2->copy)
    || ((void*)  set1->cmp    != (void*) set2->cmp)
    || ((void*)  set1->delete != (void*) set2->delete)
    || ((void*)  set1->debug  != (void*) set2->debug)){
        printf("Set's operator functions unmatching in set__fusion.\n");
        return NULL;
    }

    struct set *fusioned_set = set__empty(set1->copy, set1->delete, set1->delete, set1->debug);

    size_t i = 0;
    size_t size = set__size(set1);
    while (i < size){
        set__add(fusioned_set, set1->copy(set1->s[i]));
        i++;
    }

    i = 0;
    size = set__size(set2);
    while (i < size){
        set__add(fusioned_set, set2->copy(set2->s[i]));
        i++;
    }

    return fusioned_set;
}

void set__free(struct set *set)
{
    if (set == NULL)
        return;

    for (size_t i = 0; i < set->size; i++)
        set->delete(set->s[i]);
    free(set->s);
    free(set);
}

void set__debug(const struct set *s, int is_compact)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (s == NULL || s->s == NULL)
        printf("Set (NULL)\n");
    else {
        if (!is_compact) {
            printf("Set (capacity: %zu, size: %zu, content: {\n", s->capacity, s->size);
            size_t i = 0;
            while (i < s->size) {
                printf("\t");
                s->debug(s->s[i]);
                i++;
            }
            printf("})\n");
        } else {
            printf("{ ");
            size_t i = 0;
            while (i < s->size) {
                s->debug(s->s[i]);
                i++;
            }
            printf("} ");
        }
    }
}
