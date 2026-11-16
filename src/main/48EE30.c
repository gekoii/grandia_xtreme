/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "sdk/eekernel.h"
#include "protos/main.h"

typedef struct Item {
    u8 pad0[0x10];
    s32 flags;
    struct Item *self;
    s32 id;
    void *client;
    s32 arg;
} Item;

typedef struct {
    Item *item;
    s32 id;
    s32 sema;
    s32 xC;
    s32 x10;
    u8 pad14[0x10];
    s32 x24;
} Client;

Item *_rpc_get_packet(void *);
s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 WaitSema(s32);

s32 sceSifBindRpc(Client *c, s32 arg1, u32 flags) {
    SemaParam sp;
    Item *it;

    c->x10 = 0;
    c->x24 = 0;
    it = _rpc_get_packet(&_sceSifRpcData);
    if (it == NULL) {
        return -1;
    }
    c->item = it;
    c->id = it->id;
    it->arg = arg1;
    it->self = it;
    it->client = c;
    if (!(flags & 1)) {
        sp.maxCount = 1;
        sp.initCount = 0;
        if ((c->sema = CreateSema(&sp)) < 0) {
            _rpc_free_packet(it);
            return -3;
        }
        if (sceSifSendCmd(0x80000009, it, 0x40, 0, 0, 0) == 0) {
            _rpc_free_packet(it);
            DeleteSema(c->sema);
            return -2;
        }
        WaitSema(c->sema);
        DeleteSema(c->sema);
        return 0;
    } else {
        c->sema = -1;
        if (sceSifSendCmd(0x80000009, it, 0x40, 0, 0, 0) == 0) {
            _rpc_free_packet(it);
            return -2;
        }
        return 0;
    }
}
