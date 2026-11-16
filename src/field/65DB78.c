#include "common.h"
#include "protos/field.h"
#include "field/n26.h"


void McSaveReadDir(void) {
    McPortState *c;
    s32 n;
    McReq *info;
    McSlotTable *o;

    if (mcSaveStateChanged != 0) {
        mcDirListPtr->max = 0x10;
        mcSaveDirReq = McReqGetDir((s32)mcSaveDirPattern, 0, 0x10, (s32)mcDirListPtr->entries);
        return;
    }
    if (McIsBusy() == 0) {
        c = McGetCurPort();
        n = McReqGetStatus(mcSaveDirReq);
        info = McReqGet(mcSaveDirReq);
        o = McGetSlotTable();
        if (n > 0 && info->result >= 0) {
            McSaveSetState(0x102);
            mcDirListPtr->count = info->result;
            McSlotsMarkUsed(mcDirListPtr);
            o->dirExists = 1;
            o->fileMask = 0xF;
            mcSaveFreeKB = c->freeKB;
        } else {
            mcSaveFreeKB = c->freeKB;
            mcSaveGameFilesKB = 0;
            mcDirListPtr->count = 0;
            McSaveSetState(0x200);
            o->dirExists = 0;
            o->fileMask = 0;
            McSlotTableClear();
        }
        McReqFreeId(mcSaveDirReq);
    }
}
