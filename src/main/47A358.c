/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/libpad.h"


typedef struct {
    u8 pad0[0x50];
    u16 f50[10];
    u8 f64;
    u8 f65;
    u8 pad66[2];
    u8 f68;
    u8 f69;
    u8 pad6A[7];
    u8 f71;
    u8 f72;
} Info;


s32 scePadInfoMode(s32 arg0, s32 arg1, s32 kind, s32 idx) {
    Info *p;

    if (_scePadPortTable[arg0][arg1].open == 0) {
        return 0;
    }
    p = _scePadGetDmaStr(arg0, arg1);
    if (p->f72 != 1) {
        return 0;
    }
    if (p->f71 == 2) {
        return 0;
    }
    switch (kind) {
    case 1:
        if (p->f65 == 0xF3) {
            return 0;
        }
        return p->f65 >> 4;
    case 2:
        if (p->f64 == 1) {
            return 0;
        }
        return p->f50[p->f69];
    case 3:
        if (p->f64 == 1) {
            return 0;
        }
        return p->f69;
    case 4:
        if (p->f64 == 1) {
            return 0;
        }
        if (idx == -1) {
            return p->f68;
        }
        if (idx < p->f68) {
            return p->f50[idx];
        }
        return 0;
    }
    return 0;
}
