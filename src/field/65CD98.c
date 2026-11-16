#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "field/n26.h"


void McSaveFlowInit(void) {
    SysSaveData *g;
    s32 i;
    s32 t;

    mcSaveState = 0;
    t = ((mcIconSysSize + 0x3FF) >> 10) + 8;
    mcSavePrevState = 0xFFFFFF;
    mcSaveStateChanged = 0;
    mcSaveMaxKB = t + (((mcSaveFileEntry.size + 0x3FF) >> 10) * 10 + ((mcIconFileSize + 0x3FF) >> 10));
    mcSaveInitMask = 0xF;
    mcSaveGameFilesNeedKB = ((u32)(mcIconSysSize + 0x3FF) >> 10) + ((u32)(mcIconFileSize + 0x3FF) >> 10) + 2;
    mcSaveDone = 0;
    mcSavePort = mcLastPort;
    mcSaveSlot = mcLastSlot;
    g = &sysSaveData;
    FldMenuSaveSetLoadMode(0);
    mcSaveSlotHeader.used = 1;
    g->playTime = FldPlayTimeGet();
    for (i = 0; i < 4; i++) {
        g->headParty[i] = g->party[i];
        g->headLevel[i] = PtyCharCalcLevel(g->party[i] - 1);
    }
    McSetSaveData(&sysSaveData, 0x3300);
    McSlotCopyHeader(mcSaveFileEntry.data, &mcSaveSlotHeader);
    EvtSystemFlagSet(0x96, 0);
}
