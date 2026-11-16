#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n16.h"

extern u8 dbgChrStrGlobalNo[];
extern u8 dbgChrStrSeInterval[];
extern u8 dbgChrStrSeRandom[];
extern u8 dbgChrStrSeNormal[];
extern u8 dbgChrStrSeAttack[];

void DbgChrGroupPage(ChrObj *obj, u16 held) {
    ChrEnemyWork *d;
    FldEncGroup *in;

    if (obj != NULL) {
        d = obj->sense;
        if (d != NULL) {
            in = d->group;
            DbgPrintf(4, 4, dbgChrStrGroupMax, in->count);
            DbgPrintf(4, 5, dbgChrStrGroupPoint, in->cost);
            DbgPrintf(4, 6, dbgChrStrGroupInfHeader);
            DbgPrintf(4, 7, dbgChrStrGroupNo, in->slots[0].defNo, in->slots[1].defNo, in->slots[2].defNo, in->slots[3].defNo);
            DbgPrintf(4, 8, dbgChrStrGroupMaxRow, in->slots[0].count, in->slots[1].count, in->slots[2].count, in->slots[3].count,
                          in->slots[0].count + in->slots[1].count + in->slots[2].count + in->slots[3].count);
            DbgPrintf(4, 9, dbgChrStrGroupLevel, in->slots[0].level, in->slots[1].level, in->slots[2].level, in->slots[3].level,
                          in->slots[0].level + in->slots[1].level + in->slots[2].level + in->slots[3].level);
            DbgPrintf(4, 10, dbgChrStrGlobalNo, d->charDef->kind);
            DbgPrintf(4, 11, dbgChrStrSeInterval, (s16)d->charDef->seInterval);
            DbgPrintf(4, 12, dbgChrStrSeRandom, d->charDef->seIntervalRand);
            DbgPrintf(4, 13, dbgChrStrSeNormal, d->charDef->se);
            DbgPrintf(4, 14, dbgChrStrSeAttack, d->charDef->alertSe);
        }
    }
}
