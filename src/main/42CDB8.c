#include "common.h"
#include "protos/main.h"
#include "save.h"
#include "main/n03.h"

s32 strncmp(s8 *, void *, s32);
extern u8 mcSaveDirPath[];
extern s32 mcScanReqIds[3];
extern s32 mcScanDirIndex;
extern s32 mcScanStep;
extern s32 mcScanSlotNo;
extern SysSaveData mcSaveReadBuf;

s32 McScanSlots(s32 arg0) {
    McSlotTable *obj;
    sceMcTblGetDir *e;
    s8 *name;
    McSlot *rec;
    s32 h;
    s32 i;
    SysSaveData *p;

    obj = McGetSlotTable();
    if (arg0 == 0) {
        mcScanStep = 0;
    }
    if (McIsBusy() != 0) {
        return 0;
    }
    switch (mcScanStep) {
    case 0:
        mcScanReqIds[0] = McReqChdir(mcSaveDirPath, 0);
        mcScanDirIndex = 0;
        mcScanStep++;
        break;
    case 1:
        McReqFreeId(mcScanReqIds[0]);
        e = mcDirListPtr->entries;
        /* goto, not break: a break changes the allocation of the whole function */
        for (; mcScanDirIndex < mcDirListPtr->count; mcScanDirIndex++) {
            e = &mcDirListPtr->entries[mcScanDirIndex];
            if (strncmp(e->EntryName, mcSaveNamePrefix, 4) == 0) goto found;
        }
found:
        if (mcScanDirIndex == mcDirListPtr->count) {
            mcScanStep = 3;
            break;
        }
        name = e->EntryName;
        mcScanSlotNo = (name[4] - '0') * 10 + (name[5] - '0');
        rec = &obj->slots[mcScanSlotNo];
        rec->size = e->FileSizeByte;
        rec->used = 1;
        rec->status = 0;
        if (e->FileSizeByte != mcSaveDataSize[0]) {
            rec->status = -1;
            mcScanStep = 1;
            mcScanDirIndex++;
            break;
        }
        h = McReqOpen(name, 1);
        mcScanReqIds[0] = h;
        mcScanReqIds[1] = McReqRead(h, &mcSaveReadBuf, 0x3300);
        mcScanReqIds[2] = McReqClose(h);
        mcScanStep++;
        break;
    case 2:
        if (McReqGetStatus(mcScanReqIds[2]) > 0) {
            rec = &obj->slots[mcScanSlotNo];
            rec->status = -1;
            p = &mcSaveReadBuf;
            /* do-while(0): unwrapped, the 0x32F0 constant load is scheduled differently */
            do {
                if (p->size == 0x32F0 && McSaveCheckChecksum(p) != 0) {
                    rec->status = 0;
                    McSlotCopyHeader(p, rec);
                }
            } while (0);
            mcScanStep = 1;
            mcScanDirIndex++;
            for (i = 0; i < 3; i++) {
                McReqFreeId(mcScanReqIds[i]);
            }
        } else {
            rec = &obj->slots[mcScanSlotNo];
            rec->status = -1;
            mcScanStep = 1;
            mcScanDirIndex++;
            for (i = 0; i < 3; i++) {
                McReqFreeId(mcScanReqIds[i]);
            }
        }
        break;
    case 3:
        return 1;
    }
    return 0;
}
