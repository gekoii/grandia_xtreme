#include "common.h"
#include "protos/field.h"
#include "field/state.h"


void ChrPartyReload(void) {
    s32 i;
    ChrObj *c;

    ChrPartyLoadMember(0, 1);
    ChrPartyLoadMember(1, 0);
    ChrPartyLoadMember(2, 0);
    ChrPartyLoadMember(3, 0);
    ChrPartyLoadMember(4, 0);
    ChrPartyLoadMember(5, 0);
    ChrPartyLoadMember(6, 0);
    ChrPartyLoadMember(7, 0);
    ChrPartyLoadMember(8, 0);
    for (i = 1; i < 9; i++) {
        c = ChrGetByNo(i + 1);
        if (c != NULL && (c->flags & 0x800)) {
            c->flags ^= 0x800;
        }
    }
}
