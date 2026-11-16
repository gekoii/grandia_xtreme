#include "common.h"
#include "field/state.h"
#include "protos/field.h"

void ChrPartySlotSetActive(u8 id, u8 on) {
    ChrObj *c;

    c = ChrGetByPartySlot(id);
    if (on) {
        c->flags |= 1;
    } else if (c->flags & 1) {
        c->flags ^= 1;
    }
}
