#include "common.h"
#include "main/n03.h"
#include "protos/main.h"

extern s32 mcBusy;
extern s32 mcCmdNames[];
extern char *mcCardTypeNames[];
extern char *mcFormatNames[];
extern char mcDbgStateFmt[];
extern char mcDbgCmdFmt[];
extern char mcDbgCmdStateFmt[];
extern char mcDbgTimeFmt[];
extern char mcDbgFreeFmt[];
extern char mcDbgNoCardStr[];

void McDbgDrawCardInfo(s32 x, s32 y, McPortState *info) {
    s32 k = info->lastCmd;

    DbgPrintf(x, y, mcDbgStateFmt, mcBusy);
    DbgPrintf(x, y + 1, mcDbgCmdFmt, mcCmdNames[k]);
    DbgPrintf(x, y + 2, mcDbgCmdStateFmt, info->result);
    DbgPrintf(x, y + 3, mcDbgTimeFmt, info->busyFrames);
    if (info->state != 0) {
        DbgPrintf(x, y + 4, mcCardTypeNames[info->type]);
        DbgPrintf(x + 5, y + 4, mcFormatNames[info->formatted]);
        DbgPrintf(x, y + 5, mcDbgFreeFmt, info->freeKB);
    } else {
        DbgPrintf(x, y + 4, mcDbgNoCardStr);
    }
}
