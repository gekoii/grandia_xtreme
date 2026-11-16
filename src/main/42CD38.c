#include "common.h"
#include "main/n03.h"

void McSlotTableClear(void) {
    McSlotTable *o;
    McSlot *e;
    s32 i;

    o = McGetSlotTable();
    e = o->slots;
    for (i = 7; i >= 0; i--) {
        e->used = 0;
        e->status = 0;
        e->playTime = 0;
        e->unk2 = 0;
        e->size = 0;
        e->party[0] = 0;
        e->party[1] = 0;
        e->party[2] = 0;
        e->party[3] = 0;
        e->level[0] = 0;
        e->level[1] = 0;
        e->level[2] = 0;
        e->level[3] = 0;
        e++;
    }
    o->iconSys.used = 0;
    o->iconSys.status = 0;
    o->iconFile.used = 0;
    o->iconFile.status = 0;
    o->gameFile.used = 0;
    o->gameFile.status = 0;
}
