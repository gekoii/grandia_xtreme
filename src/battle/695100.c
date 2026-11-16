#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"


void BtlUnitInfoOpen(BtlUnit *unit, s32 b) {
    BtlUnitInfo *p = &btlUnitInfo;
    if (p->unit == NULL) {
        p->unit = unit;
        p->state = 0;
        MenuFrameInit(&p->frame, 0x130, 0x80);
        p->hidden = b;
    } else if (p->unit != unit) {
        p->unit = unit;
        p->hidden = b;
        p->state = 0;
    }
}
