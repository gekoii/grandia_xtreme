#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

s32 McSaveWriteIconFiles(s32 reset) {
    McSlotTable *o;
    s32 n;
    s32 id;

    o = McGetSlotTable();
    if (reset == 0) {
        mcSaveIconStep = 0;
    }
    if (McIsBusy() != 0) {
        return 0;
    }
    if (mcSaveIconStep == 0) {
        mcSaveIconReqs[0] = McReqChdir((s32)mcSaveFlowDirPath, 0);
        mcSaveIconStep++;
        return 0;
    }
    if (mcSaveIconStep == 1) {
        n = McReqGetStatus(mcSaveIconReqs[0]);
        McReqFreeId(mcSaveIconReqs[0]);
        if (n == 0) {
            return 0;
        }
        if (n < 0) {
            return -1;
        }
        mcSaveIconStep++;
    }
    if (mcSaveIconStep == 2) {
        if (o->iconSys.used != 0 && o->iconSys.status == 0) {
            mcSaveIconStep = 4;
        } else {
            id = McReqOpen(&mcIconSysEntry, 0x200);
            mcSaveIconReqs[0] = id;
            mcSaveIconReqs[1] = McReqWrite(id, (s32)mcIconSysEntry.data, mcIconSysEntry.size);
            mcSaveIconReqs[2] = McReqClose(id);
            mcSaveIconStep++;
            return 0;
        }
    }
    if (mcSaveIconStep == 3) {
        n = McReqGetStatus(mcSaveIconReqs[2]);
        McReqFreeId(mcSaveIconReqs[0]);
        McReqFreeId(mcSaveIconReqs[1]);
        McReqFreeId(mcSaveIconReqs[2]);
        if (n == 0) {
            return 0;
        }
        if (n < 0) {
            return -1;
        }
        mcSaveIconStep++;
    }
    if (mcSaveIconStep == 4) {
        if (o->iconFile.used != 0 && o->iconFile.status == 0) {
            mcSaveIconStep = 6;
        } else {
            id = McReqOpen(&mcIconFileEntry, 0x200);
            mcSaveIconReqs[0] = id;
            mcSaveIconReqs[1] = McReqWrite(id, (s32)mcIconFileEntry.data, mcIconFileEntry.size);
            mcSaveIconReqs[2] = McReqClose(id);
            mcSaveIconStep++;
            return 0;
        }
    }
    if (mcSaveIconStep == 5) {
        n = McReqGetStatus(mcSaveIconReqs[2]);
        McReqFreeId(mcSaveIconReqs[0]);
        McReqFreeId(mcSaveIconReqs[1]);
        McReqFreeId(mcSaveIconReqs[2]);
        if (n == 0) {
            return 0;
        }
        if (n < 0) {
            return -1;
        }
        mcSaveIconStep++;
    }
    if (mcSaveIconStep == 6) {
        if (o->gameFile.used != 0 && o->gameFile.status == 0) {
            mcSaveIconStep = 8;
        } else {
            id = McReqOpen(mcSaveGameFileName, 0x200);
            mcSaveIconReqs[0] = id;
            mcSaveIconReqs[1] = McReqClose(id);
            mcSaveIconStep++;
            return 0;
        }
    }
    if (mcSaveIconStep == 7) {
        n = McReqGetStatus(mcSaveIconReqs[1]);
        McReqFreeId(mcSaveIconReqs[0]);
        McReqFreeId(mcSaveIconReqs[1]);
        if (n == 0) {
            return 0;
        }
        if (n < 0) {
            return -1;
        }
        if (n > 0) {
            return 1;
        }
    }
    return mcSaveIconStep >= 8;
}
