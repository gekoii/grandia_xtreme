#include "common.h"
#include "party.h"

s16 PtyMemberGetLevel(s16 arg0) {
    return ptyPartyUnits[arg0].level;
}
