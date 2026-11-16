#include "common.h"
#include "sys.h"
#include "protos/field.h"

extern f32 fldPerfTimes[];
extern char dbgFldPerfStrSync[];
extern char dbgFldPerfStrOcc[];
extern char dbgFldPerfStrParticle[];
extern char dbgFldPerfStrParty[];
extern char dbgFldPerfStrGround[];
extern char dbgFldPerfStrTenant[];
extern char dbgFldPerfStrScenario[];
extern char dbgFldPerfStrHta[];
extern char dbgFldPerfStrWindow[];
extern char dbgFldPerfStrSort[];
extern char dbgFldPerfStrSky[];
extern char dbgFldPerfStrShadow[];


void DbgFldPerfPage(void) {
    DbgPrintf(4, 3, dbgFldPerfStrSync, sysPerfTimes[4] - sysPerfTimes[3]);
    DbgPrintf(4, 4, dbgFldPerfStrOcc, fldPerfTimes[1] - fldPerfTimes[0]);
    DbgPrintf(4, 5, dbgFldPerfStrParticle, fldPerfTimes[2] - fldPerfTimes[1]);
    DbgPrintf(4, 6, dbgFldPerfStrParty, fldPerfTimes[3] - fldPerfTimes[2]);
    DbgPrintf(4, 7, dbgFldPerfStrGround, fldPerfTimes[4] - fldPerfTimes[3]);
    DbgPrintf(4, 8, dbgFldPerfStrTenant, fldPerfTimes[5] - fldPerfTimes[4]);
    DbgPrintf(4, 9, dbgFldPerfStrScenario, fldPerfTimes[6] - fldPerfTimes[5]);
    DbgPrintf(4, 10, dbgFldPerfStrHta, fldPerfTimes[7] - fldPerfTimes[6]);
    DbgPrintf(4, 11, dbgFldPerfStrWindow, fldPerfTimes[8] - fldPerfTimes[7]);
    DbgPrintf(4, 12, dbgFldPerfStrSort, sysPerfTimes[5] - sysPerfTimes[4]);
    DbgPrintf(4, 13, dbgFldPerfStrGround, fldPerfTimes[10] - fldPerfTimes[9]);
    DbgPrintf(4, 14, dbgFldPerfStrParty, fldPerfTimes[11] - fldPerfTimes[10]);
    DbgPrintf(4, 15, dbgFldPerfStrTenant, fldPerfTimes[12] - fldPerfTimes[11]);
    DbgPrintf(4, 16, dbgFldPerfStrSky, fldPerfTimes[13] - fldPerfTimes[12]);
    DbgPrintf(4, 17, dbgFldPerfStrShadow, fldPerfTimes[14] - fldPerfTimes[13]);
    ((s32 (*)(s32, s32, char *, ...))DbgPrintf)(4, 18, dbgFldPerfStrParticle, fldPerfTimes[15] - fldPerfTimes[14]);
}
