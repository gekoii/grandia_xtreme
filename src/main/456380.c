#include "common.h"

u32 GsVif1GetStat(void) {
    return *(volatile u32 *)0x10003C00 & 0x1F00000F;
}
