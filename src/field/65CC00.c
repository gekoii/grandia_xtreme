#include "common.h"
#include "protos/field.h"
#include "field/n26.h"


void McSpaceCheckReadDir(void) {
    s32 n;
    McReq *req;
    s32 v;

    if (mcSpaceCheckPortStateChanged != 0) {
        mcSpaceCheckDirList->max = 0x10;
        mcSpaceCheckDirReq = McReqGetDir((s32)mcSpaceCheckDirPattern, 0, 0x10, (s32)mcSpaceCheckDirList->entries);
        return;
    }
    /* do-while(0) as a breakable block: the breaks are its forward exits, laid out as a loop */
    do {
        if (McIsBusy() != 0) {
            break;
        }
        n = McReqGetStatus(mcSpaceCheckDirReq);
        req = McReqGet(mcSpaceCheckDirReq);
        if (n > 0 && (v = req->result) >= 0) {
            mcSpaceCheckDirList->count = v;
            McSlotsMarkUsed(mcSpaceCheckDirList);
            McSpaceCheckSetPortState(0x409);
        } else {
            McSpaceCheckSetPortState(0x406);
        }
        McReqFreeId(mcSpaceCheckDirReq);
    } while (0);
}
