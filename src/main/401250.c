#include "common.h"
#include "sys.h"
#include "protos/main.h"

extern u8 dbgCursorOnStr[];
extern u8 dbgCursorOffStr[];

void DbgDrawCursor(s32 arg0, s32 arg1) {
    void *p;

    if ((u32)(sysLoopFrameCount & 0x1F) < 0x10U) {
        p = dbgCursorOnStr;
    } else {
        p = dbgCursorOffStr;
    }
    DbgPrintf(arg0, arg1, p);
}
