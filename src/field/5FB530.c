#include "common.h"
#include "field/state.h"
#include "protos/field.h"

void ChrExtraPartsModeSet(ChrObj *c, u8 mode) {
    if (c == NULL) {
        return;
    }
    switch (mode) {
    case 0:
        if (c->actFlags & 0x8000) {
            c->actFlags ^= 0x8000;
        }
        c->actFlags |= 0x20000;
        break;
    case 1:
        if (FldMapIsNoKamae() == 1 || EvtSystemFlagGet(0x84) == 1) {
            c->actFlags |= 0x8000;
        }
        if (c->actFlags & 0x20000) {
            c->actFlags ^= 0x20000;
        }
        break;
    case 2:
        if (c->actFlags & 0x8000) {
            c->actFlags ^= 0x8000;
            c->actFlags |= 0x20000;
        } else {
            c->actFlags ^= 0x20000;
            c->actFlags |= 0x8000;
        }
        break;
    }
}
