#include "common.h"

void FldMenuListFixOnRemove(s32 *arg0, s32 *arg1, u32 arg2, s32 arg3, u32 arg4) {
    s32 v;

    if (arg4 >= arg2 && arg4 < *arg1 + arg2) {
        v = *arg1;
        if (v != 0) {
            *arg1 = v - 1;
        } else if (*arg0 != 0) {
            *arg0 = *arg0 - 1;
        }
    }
}
