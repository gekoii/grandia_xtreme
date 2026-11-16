#include "common.h"

s32 GsVif1GetMark(void) {
    return *(volatile s32 *)0x10003C30;
}
