#include "common.h"
#include "party.h"

void PtyMemberGetSp(s16 arg0, s32 *arg1, s32 *arg2) {
    *arg1 = ptyPartyUnits[arg0].sp;
    *arg2 = ptyPartyUnits[arg0].maxSp;
}
