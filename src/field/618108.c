#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

extern char fldSndDbgTimeFmt[];

void FldSndDebugPrintTime(void) {
    s32 *p;

    if (SysIsDebugMode() && fldSndStreamHandle != 0xFFFFFFFF) {
        p = &fldSndStreamFrames;
        if (*p != 0 && EvtSystemFlagGet(0x84) != 0) {
            DbgPrintf(0x2C, 2, fldSndDbgTimeFmt, *p, *p / 60);
        }
    }
}
