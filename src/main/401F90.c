#include "common.h"
#include "protos/main.h"
#include "main/n01.h"

extern char dbgPtclTrack4Fmt[];

void DbgPtclPrintTrack4(s32 arg0, s16 *arg1, s32 arg2, s32 arg3) {
    if (*(s8 *)arg1 == 0) {
        DbgPrintf(arg2, arg3, dbgPtclLabelFmt, arg0);
        return;
    }
    DbgPrintf(arg2, arg3, dbgPtclTrack4Fmt, arg0, arg1[4], arg1[1], arg1[5], arg1[2], arg1[6], arg1[3], arg1[7]);
}
