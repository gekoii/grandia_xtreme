#include "common.h"

void GsVif1SetMark(s32 arg0) {
    *(volatile s32 *)0x10003C30 = arg0;
}
