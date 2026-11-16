#include "common.h"

void AmapOccSetCondFlag2(s16 *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5) {
    arg0[4] = arg1;
    arg0[5] = 5;
    arg0[6] = arg2;
    arg0[7] = arg4;
    arg0[8] = 5;
    arg0[9] = arg3;
    arg0[10] = arg5;
}
