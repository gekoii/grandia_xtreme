#include "common.h"
#include "protos/main.h"
#include "ptcl.h"
#include "main/n01.h"

extern char dbgPtclTrack3Fmt[];

void DbgPtclPrintTrack3(s32 arg0, PtclStep *info, s32 arg2, s32 arg3) {
    if (info->on == 0) {
        DbgPrintf(arg2, arg3, dbgPtclLabelFmt, arg0);
    } else {
        DbgPrintf(arg2, arg3, dbgPtclTrack3Fmt, arg0, info->time[0], info->time[1], info->time[2]);
    }
}
