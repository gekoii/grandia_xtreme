#include "common.h"

s32 GsGetTimerCount(void) {
    return *(volatile s32 *)0x10000800;
}
