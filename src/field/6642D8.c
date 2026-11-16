#include "common.h"
#include "protos/field.h"
#include "m2c_macros.h"


s32 EvtScnrVarCompare(u8 arg0, u16 arg1, u32 arg2, u8 arg3) {
    u32 temp_a2;
    u32 temp_s1;

    temp_s1 = arg3;
    temp_a2 = EvtScnrVarGet(arg0, arg1);
    switch (temp_s1) {
    case 0:
        return temp_a2 == arg2;
    case 1:
        return temp_a2 != arg2;
    case 2:
        return arg2 < temp_a2;
    case 3:
        return temp_a2 >= arg2;
    case 4:
        return temp_a2 < arg2;
    case 5:
        return arg2 >= temp_a2;
    default:
        return 0;
    }
}
