#include "common.h"
#include "save.h"



void EvtSaveMapCheckpoint(u8 arg0, s32 arg1) {
    SysSaveData *p = &sysSaveData;

    p->checkpointMapNo = p->mapNo;
    p->unk32F8 = p->mapNo;
    if (arg0) {
        p->checkpointCount++;
    } else {
        p->checkpointCount = 0;
    }
}
