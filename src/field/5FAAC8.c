#include "common.h"
#include "field/state.h"

void ChrHurtStart(ChrObj *c) {
    if (c->actFlags & 0x20) {
        return;
    }
    if (c->actFlags & 0x80) {
        c->actFlags ^= 0x80;
    }
    if (c->actFlags & 0x8) {
        c->actFlags ^= 0x8;
    }
    if (c->actFlags & 0x10) {
        c->actFlags ^= 0x10;
    }
    if (c->actFlags & 0x40) {
        c->actFlags ^= 0x40;
    }
    c->actFlags |= 0x20;
}
