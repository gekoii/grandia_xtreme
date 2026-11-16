/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u8 pad0[0x10];
    s32 w10;
} Obj;

typedef union {
    u64 v;
    struct {
        s32 lo;
        s32 hi;
    } s;
} Pos;

void func_004815B8(u64, u64);
u64 b2d(Obj *, s32 *);

void ratio(Obj *a, Obj *b) {
    s32 ta;
    s32 tb;
    Pos pa;
    Pos pb;
    s32 d;

    pa.v = b2d(a, &ta);
    pb.v = b2d(b, &tb);
    d = (ta - tb) + ((a->w10 - b->w10) << 5);
    if (d > 0) {
        pa.s.hi += d << 20;
    } else {
        pb.s.hi -= d << 20;
    }
    func_004815B8(pa.v, pb.v);
}
