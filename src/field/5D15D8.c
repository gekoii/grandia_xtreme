#include "common.h"
#include "field/state.h"
#include "field/n15.h"

typedef struct {
    u8 pad0[8];
    s8 unk8;
    u8 pad9[7];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
} Src;

extern FldEncGroup *mapEnemyGroupList;

void ChrEnemyWorkSetup(ChrObj *arg0, Src *arg1, s32 arg2) {
    ChrEnemyWork *w = arg0->sense; /* a tenant's enemy work */
    FldEncGroup *r;

    w->unk8 = arg1->unk10;
    w->fov = arg1->unk12;
    w->range = arg1->unk14;
    w->unkE = arg1->unk16;
    w->flags = 3;
    r = &mapEnemyGroupList[arg2];
    w->flagId = arg1->unk8;
    w->charDef = &fldCharDefTable[r->slots[0].defNo];
    w->group = r;
}
