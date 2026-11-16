#include "common.h"
#include "protos/main.h"
#include "ptcl.h"


/* arg1 is never set by the callers (one argument, through a cast); it only feeds the cast call below. */
s32 PtclGenCalcSize(PtclGenRequest *o, s32 arg1) {
    s32 n;
    s32 sz;
    u32 mask;
    s32 a;
    s32 b;

    mask = 0xFFFFFFF0;
    sz = (s32)o->ctrl;
    n = ((PtclCtrl *)sz)->groupCount;
    if (o->model == 0) {
        a = ((PtclCtrl *)sz)->flags & 1;
        sz = n * 0x7C0 + 0x1B0;
        sz = (sz + 15) & mask;
        sz += n * 0x5D0;
        sz = (sz + 15) & mask;
        sz += n * 4;
        sz = (sz + 15) & mask;
        if (a) {
            sz += n << 9;
            sz = (sz + 15) & mask;
        }
        if (o->animSet != 0) {
            a = n << 9;
            b = 0xF8;
            b *= n;
            goto tail; /* into the else arm: one copy of the tail, laid out there */
        }
    } else {
        sz = ((s32 (*)(s32, s32, PtclCtrl *, PtclGenRequest *))ChrModelCalcInstanceSize)((s32)o->model, arg1, (PtclCtrl *)sz, o) * n + 0x1D0;
        sz = (sz + 15) & mask;
        b = n * 0x60;
        a = n * 4;
    tail:
        sz += a;
        sz = (sz + 15) & mask;
        sz += b;
        sz = (sz + 15) & mask;
    }
    return sz;
}
