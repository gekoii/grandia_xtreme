/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u32 b0 : 1;
    u32 b1 : 1;
    u32 b2 : 2;
    u32 b4 : 4;
    u32 b8 : 1;
    u32 rest : 23;
} Bits;

typedef struct {
    union {
        volatile u32 w;
        Bits b;
    } f0;
    u8 pad4[0xC];
    u32 f10;
    u8 pad14[0xC];
    s32 f20;
} Obj;

extern u8 _sceDmaSyncTimeoutStr[];

void sceDmaRecvN(Obj *arg0, s32 arg1, s32 arg2) {
    s32 n;
    s32 v;
    s32 t;

    n = 0x1000000;
    while (arg0->f0.w & 0x100) {
        if (--n < 0) {
            printf(_sceDmaSyncTimeoutStr);
            v = arg0->f0.w;
            if (((u32)v >> 8) & 1) {
                do {
                    t = v & ~0x100;
                    v = t;
                } while (((u32)v >> 8) & 1);
                arg0->f0.w = t;
            }
        }
    }
    if (arg0->f10 != 0xFFFFFFFF) {
        arg0->f10 = arg1;
    }
    arg0->f20 = arg2;
    arg0->f0.b.b2 = 0;
    arg0->f0.b.b0 = 0;
    arg0->f0.b.b8 = 1;
}
