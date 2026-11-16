#include "common.h"

s32 GsVif0GetStat(void) {
    return *(volatile s32 *)0x10003800 & 0x0F000007;
}
