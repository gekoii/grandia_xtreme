#include "common.h"

void AmapOccSetCondFlag(s16 *arg0, u8 arg1, u8 arg2) {
    arg0[4] = 1;
    arg0[5] = 5;
    arg0[6] = arg1;
    arg0[7] = arg2;
}
