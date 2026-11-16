/* rodata: 0x006E28A0-0x006E29F0 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "pad.h"
#include "sys.h"
#include "party.h"

static const char save[] = "save";
extern s32 dbgBtlEggNo;
extern s32 dbgBtlSkillNo;


void DbgBtlDataMenu(void) {
    SysState *m;
    Pad *pad;

    switch (sysState.dbgMenuFlags & 0xF000) {
    case 0x1000:
        DbgBtlWorkItemPage();
        return;
    case 0x2000:
        DbgBtlWorkEggPage();
        return;
    case 0x3000:
        DbgBtlWorkSkillPage();
        return;
    }
    m = &sysState;
    if (!(m->dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        DbgBtlLineSelect(pad, 2);
        if (pad->trigger & 0x20) {
            switch (m->dbgCursor) {
            case 0:
                m->dbgPage = 0; m->dbgCursor = 0; m->dbgMenuFlags |= 0x1000;
                break;
            case 1:
                m->dbgPage = 0; m->dbgCursor = 0; m->dbgMenuFlags |= 0x2000;
                break;
            case 2:
                m->dbgPage = 0; m->dbgCursor = 0; m->dbgMenuFlags |= 0x3000;
                break;
            }
        }
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 2, "%sDATA%s", "\\c1 ", "\\c0 ");
    DbgPrintf(2, 3, "> ITEM");
    DbgPrintf(2, 4, "> EGG");
    DbgPrintf(2, 5, "> SKL");
}

void DbgBtlWorkItemPage(void) {
    s32 i;
    PtyItemSlot *p;

    DbgPrintf(2, 2, "%sWORK ITEM%s", "\\c1 ", "\\c0 ");
    p = ptyPartyData->bag;
    for (i = 0; i < 30; i++, p++) {
        DbgPrintf(2, i + 3, "%4.d   num %2.1d", p->id, p->count);
    }
}

void DbgBtlWorkEggPage(void) {
    s32 i;
    PtyManaEgg *e;
    s32 n;
    f32 f;

    e = &ptyPartyData->eggs[dbgBtlEggNo];
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlLineSelect(&padPort1, 0x12);
        if (sysState.dbgCursor == 0) {
            dbgBtlEggNo = (dbgBtlEggNo + DbgEditValue(0, 0, 1, 1.0f) < 0.0f) ? 23
                : (dbgBtlEggNo + DbgEditValue(0, 0, 1, 1.0f) > 23.0f) ? 0
                : (s32)(dbgBtlEggNo + DbgEditValue(0, 0, 1, 1.0f));
            e = &ptyPartyData->eggs[dbgBtlEggNo];
        } else if (sysState.dbgCursor >= 13) {
            f = DbgEditValue(3, 0, 7, 1.0f);
            e->spellUpgrades[sysState.dbgCursor - 13] ^= (s32)f;
        }
    }
    n = sysState.dbgCursor;
    if (n < 7) {
        DbgDrawCursor(1, n + 3);
    } else {
        DbgDrawCursor(0x15, n - 3);
    }
    DbgPrintf(2, 2, "%sWORK EGG%s", "\\c1 ", "\\c0 ");
    DbgPrintf(2, 3, "EGG (%d)", dbgBtlEggNo);
    if (e->itemId > 0) {
        DbgPrintf(2, 4, "item_no %d", e->itemId);
        DbgPrintf(2, 5, "id    %d", e->type);
        DbgPrintf(2, 6, "eqp_pc  %d", e->owner);
        DbgPrintf(2, 7, "mmp     %d", e->maxMp);
        DbgPrintf(2, 8, "mp      %d", e->mp);
        DbgPrintf(0x16, 4, "mgc_no");
        DbgPrintf(0x16, 10, "bit_spc");
        for (i = 0; i < 6; i++) {
            DbgPrintf(0x1E, i + 4, "%d", e->spells[i]);
            BtlDbgPrintBits(0x1E, i + 10, e->spellUpgrades[i], 7);
        }
    }
}

void DbgBtlWorkSkillPage(void) {
    PtySkill *s;
    s32 n;

    s = &ptyPartyData->skills[dbgBtlSkillNo];
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlLineSelect(&padPort1, 0);
        if (sysState.dbgCursor == 0) {
            dbgBtlSkillNo = (dbgBtlSkillNo + DbgEditValue(0, 0, 1, 1.0f) < 0.0f) ? 99
                : (dbgBtlSkillNo + DbgEditValue(0, 0, 1, 1.0f) > 99.0f) ? 0
                : (s32)(dbgBtlSkillNo + DbgEditValue(0, 0, 1, 1.0f));
            s = &ptyPartyData->skills[dbgBtlSkillNo];
        }
    }
    n = sysState.dbgCursor;
    if (n < 7) {
        DbgDrawCursor(1, n + 3);
    }
    DbgPrintf(2, 2, "%sWORK SKILL%s", "\\c1 ", "\\c0 ");
    DbgPrintf(2, 3, "SKILL (%d)", dbgBtlSkillNo);
    if (s->id) {
        DbgPrintf(2, 4, "skill_no %d", s->id);
        DbgPrintf(2, 5, "eqp_pc   %d", s->owner);
        DbgPrintf(2, 6, "lv       %d", s->level);
        DbgPrintf(2, 7, "exp      %d", s->exp);
    }
}
