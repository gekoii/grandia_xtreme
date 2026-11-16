#include "common.h"
#include "main/n03.h"

void DbgTextSetCharSize(s32 arg0, s32 arg1) {
    dbgCharW = (arg0 & 0xFF) * 0x10;
    dbgCharH = (arg1 & 0xFF) * 0x10;
}

void DbgTextSetOffset(s32 arg0, s32 arg1) {
    dbgTextOfsX = arg0;
    dbgTextOfsY = arg1;
}
