#include "common.h"
#include "party.h"

void PtyMemberGetHp(s16 arg0, s32 *arg1, s32 *arg2) {
    *arg1 = ptyPartyUnits[arg0].hp;
    *arg2 = ptyPartyUnits[arg0].maxHp;
}
