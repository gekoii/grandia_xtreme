/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    s32 u0;
    u8 pad4[4];
    s32 u8_;
    u8 padC[8];
    s32 u14;
    s32 u18;
    void (*cb)(s32);
    s32 cbarg;
    s32 u24;
} Sub;

typedef struct {
    u8 pad[0x1C];
    Sub *sub;
    u32 cmd;
    s32 a;
    s32 b;
    s32 c;
} Msg;

void iSignalSema(s32);

void _request_end(Msg *arg0) {
    Sub *s;

    switch (arg0->cmd) {
    case 0x8000000A:
        s = arg0->sub;
        if (s->cb != NULL) {
            s->cb(s->cbarg);
        }
        break;
    case 0x80000009:
        s = arg0->sub;
        s->u24 = arg0->a;
        s->u14 = arg0->b;
        s->u18 = arg0->c;
        break;
    case 0x8000000B:
        break;
    }
    s = arg0->sub;
    if (s->u8_ >= 0) {
        iSignalSema(s->u8_);
    }
    _rpc_free_packet(s->u0);
    s->u0 = 0;
}
