#ifndef COMMON_TESTS_UTILS_H
#define COMMON_TESTS_UTILS_H

#ifndef SUCCESS
#define SUCCESS 0
#endif

#define TEST_SUCCESS 0

#include "../src/common/interface.h"
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////
///     COMMON TESTS FUNCTIONS
////////////////////////////////////////////////////////////////////

/**
 * @brief Print if a test succeeded or not, in color
 * @param result the test result
 * @param nb_success the number of test successes
 * @param nb_tests the number of test executed
 */
void print_test_result(int result, int *nb_success, int *nb_tests);

/**
 * @brief Print tests results
 * @param nb_success number of successes
 * @param nb_tests number of tests
 */
void print_test_summary(int nb_success, int nb_tests);

////////////////////////////////////////////////////////////////////
///     OPERATORS FOR ADT TESTS
////////////////////////////////////////////////////////////////////

enum card_id;

void* operator_copy(const enum card_id *p_card_id);
void operator_delete(enum card_id *p_card_id);
int operator_compare(const enum card_id *ci1, const enum card_id *ci2);
void operator_debug(const enum card_id *p_card_id);
void plus_op(enum card_id *ci);

#endif