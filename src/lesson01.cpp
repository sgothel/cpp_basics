//============================================================================
// Name        : lesson00.cpp
// Author      : Sven Gothel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson 00
//============================================================================

#include <cstdio>

/**
 * Lesson 01
 *
 * Applications of a C++ function
 */

/**
 * Pyramid w/ base_len == 5
 *      X
 *     XXX
 *    XXXXX
 *
 * Pyramid w/ base_len == 5
 *     XX
 *    XXXX
 *
 * @param base_len
 */
void pyramid(const int base_len) {
    printf(" "); // space
    printf("X"); // X
    printf("\n"); // new line
    (void) base_len;
}

int main(int argc, const char* argv[]) {
    pyramid(4);
    pyramid(5);
    return 0;
}
