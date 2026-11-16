#include "common.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "sys.h"


void BtlUnitIdsSetup(void) {
    s32 i;
    s32 v;
    BtlCharDef *e;

    for (i = 0; i < 16; i++) {
        btlUnitIds.ids[i] = btlHandoff.unitIds[i];
        btlUnitIds.flags[i] = btlHandoff.unitFlags[i];
    }
    e = btlCharDefs;
    for (i = 0; i < 4; i++) {
        e->slot = i;
        e->id = btlHandoff.partyKind[i];
        e++;
    }
    btlEncounterFlags = 0;
    for (i = 0; i < 4; i++, e++) {
        v = btlHandoff.enemyKind[i];
        e->slot = i + 4;
        e->id = v;
        switch (v & 0xFF) {
        case 0x41:
            btlEncounterFlags = 2;
            break;
        case 0x51:
            btlEncounterFlags = 0x11;
            break;
        case 0x61:
            btlEncounterFlags = 0x12;
            break;
        case 0x65:
            btlEncounterFlags = 0x13;
            break;
        case 0x71:
            btlEncounterFlags = 0x14;
            break;
        case 0x81:
            btlEncounterFlags = 0x15;
            break;
        case 0x91:
            btlEncounterFlags = 0x16;
            break;
        case 0xA1:
            btlEncounterFlags = (v & 0x100) ? 0x18 : 0x17;
            break;
        case 0xC1:
            btlEncounterFlags = 0x19;
            break;
        case 0xD1:
            btlEncounterFlags = 0x1A;
            break;
        }
    }
}
