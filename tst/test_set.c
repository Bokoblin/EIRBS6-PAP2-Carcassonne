#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "test_utils.h"
#include "../src/common/ADT/set.h"
#include "../src/common/utils.h"

////////////////////////////////////////////////////////////////////
///     USER FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

void * int__copy(void * i) {
    int * _i = (int *)i;
    int * copy = safe_malloc(sizeof(*copy));
    *copy = *_i;
    return copy;
}

void int__free(void * i) {
    int * _i = (int *)i;
    free(_i);
}

int int__cmp(void * i1, void * i2) {
    int * _i1 = (int *)i1, * _i2 = (int *)i2;
    if (*_i1 > *_i2)
        return 1;
    if (*_i1 < *_i2)
        return -1;
    return 0;
}

void int__display(const void * i) {
    int * _i = (int *)i;
    printf("%d", *_i);
}

////////////////////////////////////////////////////////////////////
///     SET FUNCTIONS TESTS
////////////////////////////////////////////////////////////////////

int test_set__empty(void)
{
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp);
    assert( st != NULL );

    assert( set__is_empty(st) );

    set__free(st);

    return TEST_SUCCESS;
}

int test_set__is_empty(void)
{
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp);
    assert( st != NULL );

    assert( set__is_empty(st) );

    int v1 = 1;
    assert( !set__add(st, &v1) );
    assert( !set__is_empty(st) );

    set__free(st);

    return TEST_SUCCESS;
}

int test_set__size(void) {
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp);
    assert( st != NULL );

    assert( set__size(st) == 0 );

    int v1 = 1, v2 = 5, v3 = 8;
    assert( !set__add(st, &v1) );
    assert( !set__add(st, &v2) );
    assert( !set__add(st, &v3) );
    assert( set__size(st) == 3 );

    set__free(st);

    return TEST_SUCCESS;
}

int test_set__add(void)
{
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp);
    assert( st != NULL );

    int v1 = 3;
    assert( !set__add(st, &v1) );
    assert( set__size(st) == 1 );
    assert( set__find(st, &v1) );

    assert( set__add(st, &v1) );
    assert( set__size(st) == 1 );
    assert( set__find(st, &v1) );

    int v2 = 3;
    assert( set__add(st, &v2) );
    assert( set__size(st) == 1 );
    assert( set__find(st, &v2) );

    int v3 = 9;
    assert( !set__add(st, &v3) );
    assert( set__size(st) == 2 );
    assert( set__find(st, &v1) );
    assert( set__find(st, &v3) );

    int v4 = 5;
    assert( !set__add(st, &v4) );
    assert( set__size(st) == 3 );
    assert( set__find(st, &v1) );
    assert( set__find(st, &v3) );
    assert( set__find(st, &v4) );

    int v5 = 1;
    assert( !set__add(st, &v5) );
    assert( set__size(st) == 4 );
    assert( set__find(st, &v1) );
    assert( set__find(st, &v3) );
    assert( set__find(st, &v4) );
    assert( set__find(st, &v5) );

    set__free(st);

    return TEST_SUCCESS;
}

int test_set__remove(void)
{
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp);
    assert( st != NULL );

    int v1 = 1, v2 = 2;
    assert( !set__add(st, &v1) );
    assert( !set__add(st, &v2) );

    assert( !set__remove(st, &v1) );
    assert( set__size(st) == 1 );
    assert( !set__find(st, &v1) );

    assert( !set__remove(st, &v2) );
    assert( !set__find(st, &v2) );
    assert( set__is_empty(st) );

    int v3 = 1, v4 = 6, v5 = 4;
    assert( !set__add(st, &v3) );
    assert( !set__add(st, &v4) );
    assert( set__remove(st, &v5) );
    assert( set__find(st, &v3) );
    assert( set__find(st, &v4) );
    assert( set__size(st) == 2 );

    set__free(st);

    return TEST_SUCCESS;
}

int test_set__find(void) {
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp);
    assert( st != NULL );

    int v1 = 1;
    assert( !set__find(st, &v1) );

    int v2 = 2;
    assert( !set__add(st, &v2) );
    assert( !set__find(st, &v1) );
    assert( set__find(st, &v2) );

    assert( !set__add(st, &v1) );
    assert( set__find(st, &v1) );

    int v3 = 7;
    assert( !set__add(st, &v3) );
    assert( set__find(st, &v3) );

    set__free(st);

    return TEST_SUCCESS;
}

int test_big_set(void)
{
    printf("%s... ", __func__);
    fflush(stdout);

    const size_t set_size = 00;
    int * t = safe_malloc(set_size * sizeof(*t));
    for (size_t i = 0; i < set_size; ++i)
        t[i] = (int) i;

    struct set * st = set__empty(int__copy, int__free, int__cmp);
    assert( st != NULL );

    for (size_t j = 0; j < 3; ++j)
        for (size_t i = 0; i < set_size; ++i)
            if (i%3 == j)
                assert( !set__add(st, t+i) );

    assert( set__size(st) == set_size );

    for (size_t j = 0; j < 3; ++j)
        for (size_t i = 0; i < set_size; ++i)
            if (i%3 == j)
                assert( !set__remove(st, t+i) );

    assert( set__is_empty(st) );


    set__free(st);
    free(t);

    return TEST_SUCCESS;
}

int main()
{
    print_success(test_set__empty());
    print_success(test_set__is_empty());
    print_success(test_set__size());
    print_success(test_set__add());
    print_success(test_set__remove());
    print_success(test_set__find());
    print_success(test_big_set());

    return TEST_SUCCESS;
}
