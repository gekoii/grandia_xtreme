#include "common.h"
#include "main/n02.h"

u16 SysCommonModelGroup(s32 arg0) {
    s8 v = sysCommonModelGroups[arg0];
    if (v == -1) {
        v = 0;
    }
    return v;
}
