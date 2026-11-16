/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/libpad.h"


typedef struct {
    u8 pad0[0x40];
    u8 e[9][4];
    u8 f64;
    u8 pad65[0x6B - 0x65];
    u8 f6B;
    u8 pad6C[0x72 - 0x6C];
    u8 f72;
} Info;


s32 scePadInfoComb(s32 arg0, s32 arg1, s32 idx, s32 kind) {
    Info *p;

    if (_scePadPortTable[arg0][arg1].open == 0) {
        return 0;
    }
    p = _scePadGetDmaStr(arg0, arg1);
    if (p->f72 != 1) {
        return 0;
    }
    if (p->f64 < 2) {
        return 0;
    }
    if (idx == -1) {
        return p->f6B;
    }
    if (idx >= p->f6B) {
        return 0;
    }
    switch (kind) {
    case -1:
        return p->e[idx][0];
    case 0:
        return p->e[idx][1];
    case 1:
        return p->e[idx][2];
    case 2:
        return p->e[idx][3];
    }
    return 0;
}
