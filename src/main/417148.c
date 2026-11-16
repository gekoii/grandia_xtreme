#include "common.h"
#include "protos/main.h"


s32 TxtStringListWidth(s32 arg0, s32 *arg1, s8 arg2) {
    s32 sum;

    sum = 0;
    while (*arg1 != 0) {
        sum += TxtStringWidth(arg0, *arg1, arg2);
        arg1++;
    }
    return sum;
}
