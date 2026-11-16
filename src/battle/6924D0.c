/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/misc.h"
#include "party.h"
#include "protos/battle.h"

s32 BtlCmdListItemConsume(s32 i) {
    s32 r = 0;
    s8 *slot;
    PtyItemUse *e;
    f32 pct;

    if (i >= 4 || *(slot = &btlCmdListReservedItem[i]) < 0) {
        return 0;
    }
    e = ptyItemTable[ptyPartyData->bag[*slot].id].use;
    if (e->rate < 100) {
        pct = e->rate * 0.01f;
        if (!((rand() >> 15) * (1.0f / 65536.0f) < pct)) {
            goto skip; /* if/else forms change the frame size and block order */
        }
    }
    ptyPartyData->bag[*slot].count--;
    r = e->rate < 100;
skip:
    btlCmdListReservedItem[i] = -1;
    return r;
}
