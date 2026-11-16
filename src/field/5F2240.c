#include "common.h"
#include "party.h"

s32 PtyMemberGetNextExp(s16 arg0) {
    return ptyPartyUnits[arg0].nextExp;
}
