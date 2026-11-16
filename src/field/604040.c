#include "common.h"
#include "protos/field.h"


s32 AmapDbgItemLogPage(s32 arg0, s32 arg1, s16 arg2) {
    if (arg2 == 0) {
        return 3;
    }
    AmapDbgItemLogDraw(arg0, arg2 - 1);
    return 0;
}
