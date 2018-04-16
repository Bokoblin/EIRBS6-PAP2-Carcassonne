#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>

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


#endif