/* Port-only C for PtyMoveLevelsUpdate (0x00416A98), which the decompilation still has in assembly. */
#include "common.h"
#include "party.h"

void PtyMoveLevelsUpdate(void);

/* Rebuilds the party's move level slots from the tables. The 40 character
   moves: id and exp from the threshold table (exp from its u16 at +0x16),
   maxLevel the first level from 2 on whose exp threshold is <= 0 (or 7;
   1 if exp[1] <= 0), level 0 with no exp, else 1 plus the further levels
   whose threshold the exp reaches. The 50 combos: level and exp 0,
   maxLevel the members' average maxLevel (summed in a byte). */
void PtyMoveLevelsUpdate(void) {
    PtyMoveLevel *e = ptyPartyData->moveLevels;
    PtyMoveLevelThreshold *th = ptyMoveLevelThresholds;
    PtyCombo *c;
    s32 i;
    s32 j;
    u16 exp;

    for (i = 0; i < PTY_MOVE_LEVEL_MOVES; i++, e++, th++) {
        e->moveId = th->moveId;
        exp = *(u16 *)((u8 *)th + 0x16);
        e->maxLevel = 1;
        e->exp = exp;
        if (th->exp[1] > 0) {
            do {
                e->maxLevel++;
            } while (e->maxLevel < 7 && th->exp[e->maxLevel] > 0);
        }
        if (exp == 0) {
            e->level = 0;
            continue;
        }
        e->level = 1;
        if (1 < e->maxLevel && e->exp >= th->exp[1]) {
            do {
                e->level++;
            } while (e->level < e->maxLevel && exp >= th->exp[e->level]);
        }
    }

    c = ptyComboTable;
    for (i = 0; i < PTY_COMBO_COUNT; i++, e++, c++) {
        e->level = 0;
        e->moveId = c->moveId;
        e->exp = 0;
        e->maxLevel = 0;
        j = 0;
        while (c->chars[j] >= 0) {
            e->maxLevel += ptyPartyData->moveLevels[c->chars[j] * PTY_CHAR_MOVES + c->moves[j]].maxLevel;
            j++;
            if (j >= 4) {
                break;
            }
        }
        if (j > 0) {
            e->maxLevel = (s32)e->maxLevel / j;
        }
    }
}
