#include "set.h"
#include "../utils.h"

#define BASIC_SET_SIZE 8 //must be a power of two

////////////////////////////////////////////////////////////////////
///     SET STRUCTURE
////////////////////////////////////////////////////////////////////

struct set
{
    void **array;
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

    int cmp_res = set->cmp(searched_e, set->array[middle_e]);

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
        set->array[k]=set->array[k-1];
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
        set->array[k]=set->array[k+1];
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
    set->array = safe_malloc(sizeof(void*)*set->capacity);
    for (size_t i = 0; i < set->capacity; i++)
        set->array[i] = NULL;
    set->size = 0;
    set->copy = copy_op;
    set->delete = delete_op;
    set->cmp = compare_op;
    set->debug = debug_op;

    return set;
}

int set__is_empty(struct set const *s)
{
    assert_not_null(s, __func__, "s parameter");

    return (s->size == 0);
}

int set__add(struct set *s, void* x)
{
    assert_not_null(s, __func__, "s parameter");
    assert_not_null(s->array, __func__, "s array");

    if  (x == NULL)
        return !SUCCESS;

    size_t pos = find(s, 0, s->size, x);
    if  ((pos < s->size) && (s->cmp(s->array[pos], x) == 0))
        return !SUCCESS;

    //Increase memory if needed
    if (s->size == s->capacity - 1) {
        s->capacity = s->capacity * 2;
        s->array = realloc(s->array, sizeof(void *) * s->capacity);
        if (s->array == NULL)
            return !SUCCESS;
        else
            for (size_t i = s->capacity / 2; i < s->capacity; i++)
                s->array[i] = NULL;
    }

    //Add x into the s
    shift_right(s, pos, s->size);
    s->array[pos] = s->copy(x);
    s->size++;

    return SUCCESS;
}

int set__add_no_copy(struct set *s, void* e)
{
    assert_not_null(s, __func__, "s parameter");
    assert_not_null(s->array, __func__, "s array");

    if  (e == NULL)
        return !SUCCESS;

    size_t pos = find(s, 0, s->size, e);
    if  ((pos < s->size) && (s->cmp(s->array[pos], e) == 0))
        return !SUCCESS;

    //Increase memory if needed
    if (s->size == s->capacity - 1) {
        s->capacity = s->capacity * 2;
        s->array = realloc(s->array, sizeof(void *) * s->capacity);
        if (s->array == NULL)
            return !SUCCESS;
        else
            for (size_t i = s->capacity / 2; i < s->capacity; i++)
                s->array[i] = NULL;
    }

    //Add e into the s
    shift_right(s, pos, s->size);
    s->array[pos] = e;
    s->size++;

    return SUCCESS;
}

int set__remove(struct set *s, void* x)
{
    assert_not_null(s, __func__, "s parameter");
    assert_not_null(s->array, __func__, "s array");

    if  (x == NULL)
        return !SUCCESS;

    size_t pos = find(s, 0, s->size, x);
    if  ((s->size == 0)
            || (pos >= s->size)
            || ((pos < s->size) && (s->cmp(s->array[pos], x) != 0)))
        return !SUCCESS;

    //Remove the element
    s->delete(s->array[pos]);
    shift_left(s, pos + 1, s->size - 1);
    s->size--;

    //Remove some allocated memory if needed
    if (s->size < s->capacity / 4) {
        s->capacity = s->capacity / 4;
        s->array = realloc(s->array, sizeof(void*)*s->capacity);
        if (s->array == NULL)
            return !SUCCESS;
    }

    return SUCCESS;
}

void* set__retrieve(struct set *s, void* x)
{
    assert_not_null(s, __func__, "s parameter");
    assert_not_null(s->array, __func__, "s array");

    if  (x == NULL || s->size == 0)
        return NULL;

    size_t pos = find(s, 0, s->size, x);
    if (pos >= s->size || (pos < s->size && s->cmp(s->array[pos], x) != 0))
        return NULL;

    return s->array[pos];
}

int set__find(struct set const *s, void* x)
{
    assert_not_null(s, __func__, "s parameter");
    assert_not_null(s->array, __func__, "s array");

    size_t pos = find(s, 0, s->size, x);
    return pos < s->size && s->cmp(s->array[pos], x) == 0;
}

size_t set__size(struct set const * s)
{
    assert_not_null(s, __func__, "s parameter");
    assert_not_null(s->array, __func__, "s array");

    return s->size;
}

void* set__get_umpteenth(struct set const *s, size_t i)
{
    assert_not_null(s, __func__, "s parameter");
    assert_not_null(s->array, __func__, "s array");
    if (i >= set__size(s))
        return NULL;
    return s->copy(s->array[i]);
}

void* set__get_umpteenth_no_copy(struct set const *s, size_t i)
{
    assert_not_null(s, __func__, "s parameter");
    assert_not_null(s->array, __func__, "s array");

    if (i >= set__size(s))
        return NULL;
    return s->array[i];
}

struct set *set__fusion(struct set const *set1, struct set const *set2)
{
    assert_not_null(set1, __func__, "set1 parameter");
    assert_not_null(set1->array, __func__, "set1 array");
    assert_not_null(set2, __func__, "set2 parameter");
    assert_not_null(set2->array, __func__, "set2 array");

    if (((void*) set1->copy   != (void*) set2->copy)
            || ((void*)  set1->cmp    != (void*) set2->cmp)
            || ((void*)  set1->delete != (void*) set2->delete)
            || ((void*)  set1->debug  != (void*) set2->debug)){
        printf("Set's operator functions unmatching in set__fusion.\n");
        return NULL;
    }

    struct set *merged_set = set__empty(set1->copy, set1->delete, set1->delete, set1->debug);

    size_t i = 0;
    size_t size = set__size(set1);
    while (i < size){
        set__add(merged_set, set1->copy(set1->array[i]));
        i++;
    }

    i = 0;
    size = set__size(set2);
    while (i < size){
        set__add(merged_set, set2->copy(set2->array[i]));
        i++;
    }

    return merged_set;
}

void set__free(struct set *s)
{
    if (s == NULL)
        return;

    for (size_t i = 0; i < s->size; i++)
        s->delete(s->array[i]);
    free(s->array);
    free(s);
}

void set__debug(const struct set *s, int is_compact)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (s == NULL || s->array == NULL)
        printf("Set (NULL)\n");
    else {
        if (!is_compact) {
            printf("Set (capacity: %zu, size: %zu, content: {\n", s->capacity, s->size);
            size_t i = 0;
            while (i < s->size) {
                printf("\t");
                s->debug(s->array[i]);
                i++;
            }
            printf("})\n");
        } else {
            printf("{ ");
            size_t i = 0;
            while (i < s->size) {
                s->debug(s->array[i]);
                i++;
            }
            printf("} ");
        }
    }
}

void set__apply_to_all(struct set *s, applying_func_t f)
{
    for (size_t i = 0; i < s->size; i++)
        if (s->array[i] != NULL)
            f(s->array[i]);
}
