/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u8 pad0[8];
    s32 w8;
    u8 padC[8];
    s32 w14;
} Info;

typedef struct {
    u8 pad0[0x14];
    s32 w14;
    u8 pad18[4];
    s32 w1C;
    s32 w20;
    s32 info;
    s32 w28;
    s32 w2C;
} Pkt;

Pkt *_rpc_get_fpacket(s32);
Info *search_svdata(s32, s32);

void _request_bind(Pkt *src, s32 arg1) {
    Pkt *p;
    Info *info;
    s32 a;
    s32 b;

    p = _rpc_get_fpacket(arg1);
    b = src->w14;
    a = src->w1C;
    p->w1C = a;
    p->w14 = b;
    p->w20 = 0x80000009;
    info = search_svdata(src->w20, arg1);
    if (info == NULL) {
        p->info = 0;
        p->w28 = 0;
        p->w2C = 0;
    } else {
        p->info = (s32)info;
        p->w28 = info->w8;
        p->w2C = info->w14;
    }
    isceSifSendCmd(0x80000008, p, 0x40, 0, 0, 0);
}
