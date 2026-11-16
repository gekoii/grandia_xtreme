/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u32 b0 : 1;
    u32 b1 : 1;
    u32 b2 : 2;
    u32 b4 : 4;
    u32 lock : 1;
    u32 rest : 23;
} Bits;

typedef struct {
    union {
        u32 w;
        Bits b;
    } u;
    u8 pad4[0x1C];
    s32 unk20;
} Obj;

extern char _sceDmaSyncTimeoutStr[];

void sceDmaRecv(Obj *arg0) {
    s32 i;
    Bits v;

    i = 0x1000000;
    while (*(volatile u32 *)&arg0->u.w & 0x100) {
        if (--i < 0) {
            printf(_sceDmaSyncTimeoutStr);
            v = arg0->u.b;
            if (v.lock) {
                Bits t;
                do {
                    t = v;
                    t.lock = 0;
                    v = t;
                } while (v.lock);
                arg0->u.b = t;
            }
        }
    }
    arg0->unk20 = 0;
    arg0->u.b.b2 = 1;
    arg0->u.b.b0 = 0;
    arg0->u.b.lock = 1;
}
