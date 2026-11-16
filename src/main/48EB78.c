/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u8 pad[0x14];
    s32 x14;
    s32 x18;
    s32 x1C;
    s32 x20;
    s32 x24;
    s32 x28;
} Obj;

Obj *_rpc_get_fpacket(s32);

void _request_rdata(Obj *arg0, s32 arg1) {
    Obj *v = _rpc_get_fpacket(arg1);
    s32 a = arg0->x14;
    s32 b = arg0->x1C;
    v->x14 = a;
    v->x1C = b;
    v->x20 = 0x8000000C;
    isceSifSendCmd(0x80000008, v, 0x40, arg0->x20, arg0->x24, arg0->x28);
}
