#include "common.h"
#include "party.h"
#include "protos/field.h"

void PtyMemberGetStatusIcons(s16 id, s16 *arg1, s16 *arg2) {
    s32 n;
    s32 f;

    *arg1 = -1;
    *arg2 = -1;
    n = PtyPartyGetSlot(id);
    f = ptyPartyUnits[n].status;
    if (f & 0x8000) {
        *arg1 = 0xB;
        return;
    }
    if (f & 0x800) {
        *arg1 = 7;
    } else if (f & 0x10) {
        *arg1 = 0;
    }
    if (ptyPartyUnits[n].status & 0x1000) {
        if (*arg1 == -1) {
            *arg1 = 8;
            return;
        }
        *arg2 = 8;
    }
}
