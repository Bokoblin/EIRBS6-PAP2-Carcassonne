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

int set__add_no_copy(struct set *set, void* x)
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
    set->s[pos] = x;
    set->size++;

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

void* set__get_umpteenth(struct set const *set, size_t i)
{
    assert_not_null(set, __func__, "set parameter");
    assert_not_null(set->s, __func__, "set array");
    if (i >= set__size(set))
        return NULL;
    return set->copy(set->s[i]);
}

void* set__get_umpteenth_no_copy(struct set const *set, size_t i)
{
    assert_not_null(set, __func__, "set parameter");
    assert_not_null(set->s, __func__, "set array");

    if (i >= set__size(set))
        return NULL;
    return set->s[i];
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

void set__debug_data(const struct set *set, int is_compact)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (set == NULL || set->s == NULL)
        printf("Set (NULL)\n");
    else {
        if (!is_compact) {
            printf("Set (capacity: %zu, size: %zu, content: \n", set->capacity, set->size);
            printf("\t{ ");
            size_t i = 0;
            while (i < set->size) {
                set->debug(set->s[i]);
                i++;
            }
            printf("}\n)\n");
        } else {
            printf("{ ");
            size_t i = 0;
            while (i < set->size) {
                set->debug(set->s[i]);
                i++;
            }
            printf("} ");
        }
    }
}
