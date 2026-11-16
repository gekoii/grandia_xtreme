#include "common.h"
#include "sys.h"
#include "party.h"
#include "battle/n34.h"
#include "protos/battle.h"

extern char dbgBtlPcSaveEquipFmt[];
extern char dbgBtlPcSaveEqpSlotFmt[];
extern char dbgBtlPcSaveEqpSkillFmt[];

void DbgBtlPcSaveEquipPage(s32 idx) {
    PtyChar *d = &ptyCharData[idx];
    s32 i;
    s32 j;
    s32 k;

    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 4, dbgBtlPcSaveEquipFmt, d->equip[0]);
    for (i = 1; i < 6; i++) {
        DbgPrintf(8, i + 4, dbgBtlIntFmt, d->equip[i]);
    }
    DbgPrintf(2, 10, dbgBtlPcSaveEqpSlotFmt, d->slots[0]);
    for (j = 1; j < 6; j++) {
        DbgPrintf(11, j + 10, dbgBtlIntFmt, d->slots[j]);
    }
    DbgPrintf(0x16, 4, dbgBtlPcSaveEqpSkillFmt, d->skills[0]);
    for (k = 1; k < 15; k++) {
        DbgPrintf(0x20, k + 4, dbgBtlIntFmt, d->skills[k]);
    }
}
