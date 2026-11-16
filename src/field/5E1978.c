#include "common.h"
#include "field/state.h"
#include "field/n17.h"

void ChrTenantSetFlagsFromDef(ChrObj *arg0, ChrTenantDef *arg1) {
    s16 f;
    u16 a, b, c, d;

    f = arg1->flags;
    arg0->actFlags = 1;
    a = 0xCAF & ~(f & 1);
    b = a & 0xC8F; if (!(f & 2)) b = a;
    c = b & 0xCAD; if (!(f & 4)) c = b;
    d = c & 0xCAB; if (!(f & 0x20)) d = c;
    arg0->flags = d;
    arg0->flags |= arg1->mode << 4;
    arg0->flags |= 0x1000;
    if (arg1->flags & 0x80) {
        arg0->flags &= ~0x200;
    } else {
        arg0->flags |= 0x200;
    }
    if (arg1->flags & 8) arg0->flags &= ~8;
    if (arg1->flags & 4) arg0->flags &= ~0x400;
    if (arg1->flags & 0x40) arg0->flags &= ~0x800;
    if (arg1->flags & 0x100) arg0->actFlags |= 2;
    if (arg0->flags & 1) {
        arg0->flags |= 2;
    } else {
        arg0->flags &= ~2;
    }
}
