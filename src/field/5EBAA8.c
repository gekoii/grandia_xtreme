/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "party.h"
#include "protos/field.h"

#define ROUND(x) ((x) == (s32)(x) ? (x) : ((x) > 0 ? (s32)(x) + 1 : ~(s32)(-x)))







s16 ItemMoveGetCost(s16 unit, s16 skill) {
    PtyMove *sk;
    s32 u;
    s32 lv;
    s32 r;

    sk = &ptyMoveTable[skill & 0xFFF];
    u = PtyPartyGetSlot(unit);
    if (ptyPartyUnits[u].id == 0) {
        return 0;
    }
    lv = ptyPartyData->moveLevels[ItemMoveSlotIndex(skill)].level;
    if (lv != 0) {
        r = ROUND(sk->levels[lv - 1].cost * ptyPartyUnits[u].spcSpRate);
    } else {
        r = 0;
    }
    return r;
}
