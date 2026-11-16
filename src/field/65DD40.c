#include "common.h"
#include "protos/field.h"
#include "field/n26.h"


void McSaveCheckGameFiles(void) {
    sceMcTblGetDir *e;
    McSlotTable *o;
    McGameFileStat *sl;
    s32 mask;
    s32 cnt;
    u32 kb;

    mask = 0;
    cnt = 0;
    kb = 0;
    e = McSaveFindDirEntry(mcSaveIconSysName);
    o = McGetSlotTable();
    sl = &o->iconSys;
    sl->used = 0;
    if (e != NULL) {
        sl->used = 1;
        cnt++;
        sl->size = e->FileSizeByte;
        sl->status = -1;
        kb += (u32)(sl->size + 0x3FF) >> 10;
        if ((e->AttrFile & 0x80) && e->FileSizeByte == mcIconSysSize) {
            sl->status = 0;
            mask |= 1;
        }
    }
    e = McSaveFindDirEntry(mcSaveIconFileName);
    /* do-while(0): pads the label after the block with an alignment nop */
    do {
        sl = &o->iconFile;
        sl->used = 0;
        if (e != NULL) {
            sl->used = 1;
            cnt++;
            sl->size = e->FileSizeByte;
            sl->status = -1;
            kb += (u32)(sl->size + 0x3FF) >> 10;
            if ((e->AttrFile & 0x80) && e->FileSizeByte == mcIconFileSize) {
                sl->status = 0;
                mask |= 2;
            }
        }
    } while (0);
    e = McSaveFindDirEntry(mcSaveGameFileName);
    /* do-while(0): pads the label after the block with an alignment nop */
    do {
        sl = &o->gameFile;
        sl->used = 0;
        if (e != NULL) {
            sl->used = 1;
            cnt++;
            sl->size = e->FileSizeByte;
            sl->status = -1;
            kb += (u32)(sl->size + 0x3FF) >> 10;
            if ((e->AttrFile & 0x80) && e->FileSizeByte == 0) {
                sl->status = 0;
                mask |= 4;
            }
        }
    } while (0);
    mcSaveGameFilesKB = kb + ((cnt + 1) >> 1);
    (McGetSlotTable())->fileMask = mask;
    if (mask == 7) {
        McSaveSetState(0x104);
        return;
    }
    McSaveSetState(0x201);
}
