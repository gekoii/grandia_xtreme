#include "common.h"
#include "main/n03.h"
#include "protos/main.h"

void DbgTextReset(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 i;

    DbgTextClear();
    DbgTextSetCharSize(12, 12);
    for (i = 0; i < 16; i++) {
        DbgTextSetColor(i & 0xFF, 0x80808080ULL);
    }
    dbgTextOfsX = 0;
    dbgTextOfsY = 0;
}
