#include "common.h"
#include "sys.h"
#include "protos/main.h"

extern char dbgPerfStrPerformanceMenu[];
extern char dbgPerfStrStartFrame[];
extern char dbgPerfStrDrawProgram[];
extern char dbgPerfStrFbEffect[];
extern char dbgPerfStrSyncProgram[];
extern char dbgPerfStrSortProgram[];
extern char dbgPerfStrGrxEffect[];
extern char dbgPerfStrDebugDisp[];
extern char dbgPerfStrEndFrame[];
extern char dbgPerfStrSetParam[];
extern char dbgPerfStrFrameSkip[];

void DbgPerfPage(void) {
    DbgPrintf(2, 3, dbgPerfStrPerformanceMenu);
    DbgPrintf(2, 4, dbgPerfStrStartFrame, sysPerfTimes[1] - sysPerfTimes[0]);
    DbgPrintf(2, 5, dbgPerfStrDrawProgram, sysPerfTimes[2] - sysPerfTimes[1]);
    DbgPrintf(2, 6, dbgPerfStrFbEffect, sysPerfTimes[3] - sysPerfTimes[2]);
    DbgPrintf(2, 7, dbgPerfStrSyncProgram, sysPerfTimes[4] - sysPerfTimes[3]);
    DbgPrintf(2, 8, dbgPerfStrSortProgram, sysPerfTimes[5] - sysPerfTimes[4]);
    DbgPrintf(2, 9, dbgPerfStrGrxEffect, sysPerfTimes[6] - sysPerfTimes[5]);
    DbgPrintf(2, 10, dbgPerfStrDebugDisp, sysPerfTimes[7] - sysPerfTimes[6]);
    DbgPrintf(2, 11, dbgPerfStrEndFrame, sysPerfTimes[8] - sysPerfTimes[7]);
    DbgPrintf(2, 12, dbgPerfStrSetParam, sysPerfTimes[9] - sysPerfTimes[8]);
    /* through a cast: a direct call becomes a tail call (the original uses jal) */
    ((s32 (*)(s32, s32, const char *, ...))DbgPrintf)(2, 13, dbgPerfStrFrameSkip, sysPerfTimes[10] - sysPerfTimes[9]);
}
