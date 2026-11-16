#include "common.h"

s32 UtilIntToDigits(s32 arg0, u8 *arg1) {
    s32 n;

    n = arg0;
    arg1[7] = 0;
    arg1[6] = 0;
    arg1[5] = 0;
    arg1[4] = 0;
    arg1[3] = 0;
    arg1[2] = 0;
    arg1[1] = 0;
    arg1[0] = 0;
    while (n >= 10000000) {
        n -= 10000000;
        (*arg1)++;
    }
    arg1++;
    while (n >= 1000000) {
        n -= 1000000;
        (*arg1)++;
    }
    arg1++;
    while (n >= 100000) {
        n -= 100000;
        (*arg1)++;
    }
    arg1++;
    while (n >= 10000) {
        n -= 10000;
        (*arg1)++;
    }
    arg1++;
    while (n >= 1000) {
        n -= 1000;
        (*arg1)++;
    }
    arg1++;
    while (n >= 100) {
        n -= 100;
        (*arg1)++;
    }
    arg1++;
    while (n >= 10) {
        n -= 10;
        (*arg1)++;
    }
    arg1++;
    *arg1 = n;
    if (arg0 >= 10000000) {
        return 0;
    }
    if (arg0 >= 1000000) {
        return 1;
    }
    if (arg0 >= 100000) {
        return 2;
    }
    if (arg0 >= 10000) {
        return 3;
    }
    if (arg0 >= 1000) {
        return 4;
    }
    if (arg0 >= 100) {
        return 5;
    }
    if (arg0 >= 10) {
        return 6;
    }
    return 7;
}
