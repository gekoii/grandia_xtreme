#include "common.h"
#include "sys.h"
#include "protos/field.h"

extern u8 dbgFlagEditStrMarkV[];
extern u8 dbgFlagEditStrMarkBar[];

void DbgFldBitCursorDraw(s32 arg0, s32 arg1) {
    void *p;

    if ((u32)(sysLoopFrameCount & 0x1F) < 0x10U) {
        p = dbgFlagEditStrMarkV;
    } else {
        p = dbgFlagEditStrMarkBar;
    }
    DbgPrintf(arg0, arg1, p);
}
