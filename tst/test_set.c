#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "test_utils.h"
#include "../src/common/ADT/set.h"
#include "../src/common/utils.h"

/*
This test is based on Bordeaux INP's Moodle -> pg 116 -> bigtests.tar.gz
It has been modified to fit with our tests conventions.
*/

////////////////////////////////////////////////////////////////////
///     OPERATORS FUNCTIONS IMPLEMENTATION
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
    printf("%d ", *_i);
}


////////////////////////////////////////////////////////////////////
///     SET FUNCTIONS TESTS
////////////////////////////////////////////////////////////////////

int test_set__empty(void)
{
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp);
    if (st == NULL) return !TEST_SUCCESS;

    if (!( set__is_empty(st))) return !TEST_SUCCESS;

    set__free(st);

    return TEST_SUCCESS;
}

int test_set__is_empty(void)
{
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp);
    if (st == NULL) return !TEST_SUCCESS;

    if (!( set__is_empty(st))) return !TEST_SUCCESS;

    int v1 = 1;
    if (set__add(st, &v1)) return !TEST_SUCCESS;
    if (set__is_empty(st)) return !TEST_SUCCESS;

    set__free(st);

    return TEST_SUCCESS;
}

int test_set__size(void) 
{
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp);
    if (st == NULL) return !TEST_SUCCESS;

    if (set__size(st) != 0) return !TEST_SUCCESS;

    int v1 = 1, v2 = 5, v3 = 8;
    if (set__add(st, &v1)) return !TEST_SUCCESS;
    if (set__add(st, &v2)) return !TEST_SUCCESS;
    if (set__add(st, &v3)) return !TEST_SUCCESS;
    if (set__size(st) != 3) return !TEST_SUCCESS;

    set__free(st);

    return TEST_SUCCESS;
}

int test_set__add(void)
{
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp);
    if (st == NULL) return !TEST_SUCCESS;

    int v1 = 3;
    if (set__add(st, &v1)) return !TEST_SUCCESS;
    if (set__size(st) != 1) return !TEST_SUCCESS;
    if (!( set__find(st, &v1))) return !TEST_SUCCESS;

    if (!( set__add(st, &v1))) return !TEST_SUCCESS;
    if (set__size(st) != 1) return !TEST_SUCCESS;
    if (!( set__find(st, &v1))) return !TEST_SUCCESS;

    int v2 = 3;
    if (!( set__add(st, &v2))) return !TEST_SUCCESS;
    if (set__size(st) != 1) return !TEST_SUCCESS;
    if (!( set__find(st, &v2))) return !TEST_SUCCESS;

    int v3 = 9;
    if (set__add(st, &v3)) return !TEST_SUCCESS;
    if (set__size(st) != 2) return !TEST_SUCCESS;
    if (!( set__find(st, &v1))) return !TEST_SUCCESS;
    if (!( set__find(st, &v3))) return !TEST_SUCCESS;

    int v4 = 5;
    if (set__add(st, &v4)) return !TEST_SUCCESS;
    if (set__size(st) != 3) return !TEST_SUCCESS;
    if (!( set__find(st, &v1))) return !TEST_SUCCESS;
    if (!( set__find(st, &v3))) return !TEST_SUCCESS;
    if (!( set__find(st, &v4))) return !TEST_SUCCESS;

    int v5 = 1;
    if (set__add(st, &v5)) return !TEST_SUCCESS;
    if (set__size(st) != 4) return !TEST_SUCCESS;
    if (!( set__find(st, &v1))) return !TEST_SUCCESS;
    if (!( set__find(st, &v3))) return !TEST_SUCCESS;
    if (!( set__find(st, &v4))) return !TEST_SUCCESS;
    if (!( set__find(st, &v5))) return !TEST_SUCCESS;

    set__free(st);

    return TEST_SUCCESS;
}

int test_set__remove(void)
{
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp);
    if (st == NULL) return !TEST_SUCCESS;

    int v1 = 1, v2 = 2;
    if (set__add(st, &v1)) return !TEST_SUCCESS;
    if (set__add(st, &v2)) return !TEST_SUCCESS;

    if (set__remove(st, &v1)) return !TEST_SUCCESS;
    if (set__size(st) != 1) return !TEST_SUCCESS;
    if (set__find(st, &v1)) return !TEST_SUCCESS;

    if (set__remove(st, &v2)) return !TEST_SUCCESS;
    if (set__find(st, &v2)) return !TEST_SUCCESS;
    if (!( set__is_empty(st))) return !TEST_SUCCESS;

    int v3 = 1, v4 = 6, v5 = 4;
    if (set__add(st, &v3)) return !TEST_SUCCESS;
    if (set__add(st, &v4)) return !TEST_SUCCESS;
    if (!( set__remove(st, &v5))) return !TEST_SUCCESS;
    if (!( set__find(st, &v3))) return !TEST_SUCCESS;
    if (!( set__find(st, &v4))) return !TEST_SUCCESS;
    if (set__size(st) != 2) return !TEST_SUCCESS;

    set__free(st);

    return TEST_SUCCESS;
}

int test_set__find(void) {
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp);
    if (st == NULL) return !TEST_SUCCESS;

    int v1 = 1;
    if (set__find(st, &v1)) return !TEST_SUCCESS;

    int v2 = 2;
    if (set__add(st, &v2)) return !TEST_SUCCESS;
    if (set__find(st, &v1)) return !TEST_SUCCESS;
    if (!( set__find(st, &v2))) return !TEST_SUCCESS;

    if (set__add(st, &v1)) return !TEST_SUCCESS;
    if (!( set__find(st, &v1))) return !TEST_SUCCESS;

    int v3 = 7;
    if (set__add(st, &v3)) return !TEST_SUCCESS;
    if (!( set__find(st, &v3))) return !TEST_SUCCESS;

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
    if (st == NULL)
        return !TEST_SUCCESS;

    for (size_t j = 0; j < 3; ++j)
        for (size_t i = 0; i < set_size; ++i)
            if (i%3 == j)
                if (set__add(st, t+i))
                    return !TEST_SUCCESS;

    if (set__size(st) != set_size)
        return !TEST_SUCCESS;

    for (size_t j = 0; j < 3; ++j)
        for (size_t i = 0; i < set_size; ++i)
            if (i%3 == j)
                if (set__remove(st, t+i))
                    return !TEST_SUCCESS;

    if (!( set__is_empty(st)))
        return !TEST_SUCCESS;


    set__free(st);
    free(t);

    return TEST_SUCCESS;
}

//TODO : Write some tests for the following functions

int test_set__retrieve(){

    printf("%s... ", __func__);
    fflush(stdout);
    printf("NOT DONE - ");

    return !TEST_SUCCESS;
}

int test_set__get_i_th(void){

    printf("%s... ", __func__);
    fflush(stdout);
    printf("NOT DONE - ");

    return !TEST_SUCCESS;
}

int test_set__filter(void){

    printf("%s... ", __func__);
    fflush(stdout);
    printf("NOT DONE - ");

    return !TEST_SUCCESS;
}

int test_set__debug_data(void){

    printf("%s... ", __func__);
    fflush(stdout);
    printf("NOT DONE - ");

    return !TEST_SUCCESS;
}

int main()
{
    printf("----------- TEST SET -----------\n");

    print_success(test_set__empty());
    print_success(test_set__is_empty());
    print_success(test_set__size());
    print_success(test_set__add());
    print_success(test_set__remove());
    print_success(test_set__find());
    print_success(test_big_set());
    print_success(test_set__retrieve());
    print_success(test_set__get_i_th());
    print_success(test_set__filter());
    print_success(test_set__debug_data());

    return TEST_SUCCESS;
}
