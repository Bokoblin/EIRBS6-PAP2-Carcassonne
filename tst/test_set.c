#include "common_tests_utils.h"
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


//TODO: LOTS TO FREE MEMORY

int test_set__empty(void)
{
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp, int__display);
    if ((st == NULL) || (!set__is_empty(st))) {
        set__free(st);
        return !TEST_SUCCESS;
    }

    set__free(st);
    return TEST_SUCCESS;
}

int test_set__is_empty(void)
{
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp, int__display);
    if ((st == NULL) || (!set__is_empty(st)))
        return !TEST_SUCCESS;

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

    struct set * st = set__empty(int__copy, int__free, int__cmp, int__display);
    if ((st == NULL) || (set__size(st) != 0))
        return !TEST_SUCCESS;

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

    struct set * st = set__empty(int__copy, int__free, int__cmp, int__display);
    if (st == NULL) return !TEST_SUCCESS;

    int v1 = 3;
    if (set__add(st, &v1)) return !TEST_SUCCESS;
    if (set__size(st) != 1) return !TEST_SUCCESS;
    if (!set__find(st, &v1)) return !TEST_SUCCESS;

    if (!set__add(st, &v1)) return !TEST_SUCCESS;
    if (set__size(st) != 1) return !TEST_SUCCESS;
    if (!set__find(st, &v1)) return !TEST_SUCCESS;

    int v2 = 3;
    if (!set__add(st, &v2)) return !TEST_SUCCESS;
    if (set__size(st) != 1) return !TEST_SUCCESS;
    if (!set__find(st, &v2)) return !TEST_SUCCESS;

    int v3 = 9;
    if (set__add(st, &v3)) return !TEST_SUCCESS;
    if (set__size(st) != 2) return !TEST_SUCCESS;
    if (!set__find(st, &v1)) return !TEST_SUCCESS;
    if (!set__find(st, &v3)) return !TEST_SUCCESS;

    int v4 = 5;
    if (set__add(st, &v4)) return !TEST_SUCCESS;
    if (set__size(st) != 3) return !TEST_SUCCESS;
    if (!set__find(st, &v1)) return !TEST_SUCCESS;
    if (!set__find(st, &v3)) return !TEST_SUCCESS;
    if (!set__find(st, &v4)) return !TEST_SUCCESS;

    int v5 = 1;
    if (set__add(st, &v5)) return !TEST_SUCCESS;
    if (set__size(st) != 4) return !TEST_SUCCESS;
    if (!set__find(st, &v1)) return !TEST_SUCCESS;
    if (!set__find(st, &v3)) return !TEST_SUCCESS;
    if (!set__find(st, &v4)) return !TEST_SUCCESS;
    if (!set__find(st, &v5)) return !TEST_SUCCESS;

    set__free(st);

    return TEST_SUCCESS;
}

int test_set__remove(void)
{
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp, int__display);
    if (st == NULL) return !TEST_SUCCESS;

    int v1 = 1, v2 = 2;
    if (set__add(st, &v1)) return !TEST_SUCCESS;
    if (set__add(st, &v2)) return !TEST_SUCCESS;

    if (set__remove(st, &v1)) return !TEST_SUCCESS;
    if (set__size(st) != 1) return !TEST_SUCCESS;
    if (set__find(st, &v1)) return !TEST_SUCCESS;

    if (set__remove(st, &v2)) return !TEST_SUCCESS;
    if (set__find(st, &v2)) return !TEST_SUCCESS;
    if (!set__is_empty(st)) return !TEST_SUCCESS;

    int v3 = 1, v4 = 6, v5 = 4;
    if (set__add(st, &v3)) return !TEST_SUCCESS;
    if (set__add(st, &v4)) return !TEST_SUCCESS;
    if (!set__remove(st, &v5)) return !TEST_SUCCESS;
    if (!set__find(st, &v3)) return !TEST_SUCCESS;
    if (!set__find(st, &v4)) return !TEST_SUCCESS;
    if (set__size(st) != 2) return !TEST_SUCCESS;

    set__free(st);

    return TEST_SUCCESS;
}

int test_set__find(void) {
    printf("%s... ", __func__);
    fflush(stdout);

    struct set * st = set__empty(int__copy, int__free, int__cmp, int__display);
    if (st == NULL) return !TEST_SUCCESS;

    int v1 = 1;
    if (set__find(st, &v1)) return !TEST_SUCCESS;

    int v2 = 2;
    if (set__add(st, &v2)) return !TEST_SUCCESS;
    if (set__find(st, &v1)) return !TEST_SUCCESS;
    if (!set__find(st, &v2)) return !TEST_SUCCESS;

    if (set__add(st, &v1)) return !TEST_SUCCESS;
    if (!set__find(st, &v1)) return !TEST_SUCCESS;

    int v3 = 7;
    if (set__add(st, &v3)) return !TEST_SUCCESS;
    if (!set__find(st, &v3)) return !TEST_SUCCESS;

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

    struct set * st = set__empty(int__copy, int__free, int__cmp, int__display);
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

    if (!set__is_empty(st))
        return !TEST_SUCCESS;


    set__free(st);
    free(t);

    return TEST_SUCCESS;
}

int test_set__retrieve(){

    printf("%s... ", __func__);
    fflush(stdout);

    int v0 = 0, v1 = 1, v2 = 2, v3 = 3, v4 = 4, v5 = 5, v6 = 6;

    struct set *set = set__empty(int__copy, int__free, int__cmp, int__display);
    if (set__add(set, &v0)) return !TEST_SUCCESS;
    if (set__add(set, &v1)) return !TEST_SUCCESS;
    if (set__add(set, &v2)) return !TEST_SUCCESS;
    if (set__add(set, &v3)) return !TEST_SUCCESS;
    if (set__add(set, &v4)) return !TEST_SUCCESS;
    if (set__add(set, &v5)) return !TEST_SUCCESS;
    if (set__add(set, &v6)) return !TEST_SUCCESS;

    int nope_you_re_not_in_the_set = 101;

    if (set__retrieve(set, &nope_you_re_not_in_the_set) != NULL)
        return !TEST_SUCCESS;

    int *retrieved = set__retrieve(set, &v4);
    //set__debug_data(set);
    *retrieved = 69;
    int v69 = 69;
    //set__debug_data(set);

    if (!set__find(set, &v69) //FIXME: 69 IS in the set
            || set__find(set, &v4)) {
        set__free(set);
        return !TEST_SUCCESS;
    }
    set__free(set);

    return TEST_SUCCESS;
}

int test_set__get_umpteenth(void){

    printf("%s... ", __func__);
    fflush(stdout);

    int v0 = 0, v1 = 1, v2 = 2, v3 = 3, v4 = 4, v5 = 5, v6 = 6;

    struct set *set = set__empty(int__copy, int__free, int__cmp, int__display);
    if (set__add(set, &v0)) return !TEST_SUCCESS;
    if (set__add(set, &v1)) return !TEST_SUCCESS;
    if (set__add(set, &v2)) return !TEST_SUCCESS;
    if (set__add(set, &v3)) return !TEST_SUCCESS;
    if (set__add(set, &v4)) return !TEST_SUCCESS;
    if (set__add(set, &v5)) return !TEST_SUCCESS;
    if (set__add(set, &v6)) return !TEST_SUCCESS;

    int * _v0 = set__get_umpteenth(set, 0);
    int * _v1 = set__get_umpteenth(set, 1);
    int * _v2 = set__get_umpteenth(set, 2);
    int * _v3 = set__get_umpteenth(set, 3);
    int * _v4 = set__get_umpteenth(set, 4);
    int * _v5 = set__get_umpteenth(set, 5);
    int * _v6 = set__get_umpteenth(set, 6);

    if ((*_v0 != v0)
            || (*_v1 != v1)
            || (*_v2 != v2)
            || (*_v3 != v3)
            || (*_v4 != v4)
            || (*_v5 != v5)
            || (*_v6 != v6)) {
        set__free(set);
        return !TEST_SUCCESS;
    }

    *_v0 = 69;
    int v69 = 69;
    if (set__find(set, &v69)) {
        set__free(set);
        return !TEST_SUCCESS;
    }

    set__free(set);

    return TEST_SUCCESS;
}

int test_set__get_umpteenth_no_copy(void){

    printf("%s... ", __func__);
    fflush(stdout);

    int v0 = 0, v1 = 1, v2 = 2, v3 = 3, v4 = 4, v5 = 5, v6 = 6;

    struct set *set = set__empty(int__copy, int__free, int__cmp, int__display);
    if (set__add(set, &v0)) return !TEST_SUCCESS;
    if (set__add(set, &v1)) return !TEST_SUCCESS;
    if (set__add(set, &v2)) return !TEST_SUCCESS;
    if (set__add(set, &v3)) return !TEST_SUCCESS;
    if (set__add(set, &v4)) return !TEST_SUCCESS;
    if (set__add(set, &v5)) return !TEST_SUCCESS;
    if (set__add(set, &v6)) return !TEST_SUCCESS;

    int *_v0 = set__get_umpteenth_no_copy(set, 0);
    int *_v1 = set__get_umpteenth_no_copy(set, 1);
    int *_v2 = set__get_umpteenth_no_copy(set, 2);
    int *_v3 = set__get_umpteenth_no_copy(set, 3);
    int *_v4 = set__get_umpteenth_no_copy(set, 4);
    int *_v5 = set__get_umpteenth_no_copy(set, 5);
    int *_v6 = set__get_umpteenth_no_copy(set, 6);

    if ((*_v0 != v0) || (*_v1 != v1) || (*_v2 != v2) || (*_v3 != v3)
            || (*_v4 != v4) || (*_v5 != v5) || (*_v6 != v6)) {
        set__free(set);
        return !TEST_SUCCESS;
    }

    //set__debug_data(set);
    *_v0 = 69;
    int v69 = 69;
    //set__debug_data(set);

    if (!set__find(set, &v69) //FIXME: 69 IS in the set
            || set__find(set, &v0)) {
        set__free(set);
        return !TEST_SUCCESS;
    }

    set__free(set);

    return TEST_SUCCESS;
}

int test_set__debug_data(void){

    printf("%s... ", __func__);
    fflush(stdout);

    struct set *set = set__empty(int__copy, int__free, int__cmp, int__display);

    int v0 = 0,  v3 = 3, v4 = 4, v7 = 7;

    if (set__add(set, &v0)) return !TEST_SUCCESS;
    if (set__add(set, &v3)) return !TEST_SUCCESS;
    if (set__add(set, &v4)) return !TEST_SUCCESS;
    if (set__add(set, &v7)) return !TEST_SUCCESS;

    printf("(expected: 0 3 4 7) ");
    set__debug_data(set, true);

    set__free(set);

    return TEST_SUCCESS;
}

int main()
{
    printf("----------- TEST SET -----------\n");

    int nb_success = 0;
    int nb_tests = 0;

    print_test_result(test_set__empty(), &nb_success, &nb_tests);
    print_test_result(test_set__is_empty(), &nb_success, &nb_tests);
    print_test_result(test_set__size(), &nb_success, &nb_tests);
    print_test_result(test_set__add(), &nb_success, &nb_tests);
    print_test_result(test_set__remove(), &nb_success, &nb_tests);
    print_test_result(test_set__find(), &nb_success, &nb_tests);
    print_test_result(test_big_set(), &nb_success, &nb_tests);
    print_test_result(test_set__retrieve(), &nb_success, &nb_tests);
    print_test_result(test_set__get_umpteenth(), &nb_success, &nb_tests);
    print_test_result(test_set__get_umpteenth_no_copy(), &nb_success, &nb_tests);
    print_test_result(test_set__debug_data(), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
