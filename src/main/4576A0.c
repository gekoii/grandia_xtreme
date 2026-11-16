#include "common.h"
#include "protos/main.h"

void GsMfifoInit(s32 arg0, s32 arg1) {
    *(volatile u32 *)0x700000F0 = 0x70000A00;
    GsMfifoStart(arg0, arg1);
}
