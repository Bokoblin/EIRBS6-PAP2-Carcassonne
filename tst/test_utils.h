#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include "../src/common/common_interface.h"

#define TEST_SUCCESS 0

/**
 * @brief Print if a test succeeded or not, in color
 * @param result the test result
 */
void print_success(int result)
{
    if (result)
        printf("\x1B[31mFAILED\x1B[0m\n");
    else
        printf("\x1B[32mSUCCESS\x1B[0m\n");
}

////////////////////////////////////////////////////////////////////
///     OPERATORS FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

void* operator_copy(const enum card_id *p_card_id)
{
    if (p_card_id == NULL)
        return NULL;

    enum card_id *new_card = malloc(sizeof(enum card_id));
    if (new_card == NULL) return NULL;
    *new_card = *p_card_id;
    return new_card;
}

void operator_delete(enum card_id *p_card_id)
{
    free(p_card_id);
}

void operator_debug(const enum card_id *p_card_id)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (p_card_id == NULL)
        printf("NULL ");
    else
        printf("%d ", *p_card_id);
}


#endif