/* cflags: -fno-strength-reduce */
/* -fno-strength-reduce: the loops index ptyComboTable/moveLevels (with it on, they get reduced pointers and a smaller frame) */
#include "common.h"
#include "party.h"




void BtlComboLevelsUpdate(void) {
    s32 i;
    s32 j;
    s32 n;
    s32 sum;
    PtyMoveLevel *s;
    PtyCombo *d;
    s8 *pb;
    s8 k;
    s32 t;

    s = &ptyPartyData->moveLevels[PTY_MOVE_LEVEL_MOVES];
    for (i = 0; i < 50; i++, s++) {
        if (s->level != 0) {
            sum = 0;
            n = 0;
            d = &ptyComboTable[i];
            pb = d->moves;
            for (j = 0; j < 4; j++, pb++) {
                k = d->chars[j];
                if (k >= 0) {
                    n++;
                    t = k * 5 + *pb;
                    sum += ptyPartyData->moveLevels[t].level;
                }
            }
            if (n > 0) {
                s->level = sum / n;
            }
        }
    }
}
