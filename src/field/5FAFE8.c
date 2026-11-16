#include "common.h"
#include "field/state.h"
#include "protos/field.h"

void ChrLeaderOnlyActivate(void) {
    ChrObj *c;

    ChrAllDeactivate();
    c = ChrGetByPartySlot(1);
    c->flags |= 1;
}
