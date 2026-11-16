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
    s32 arg2;
    s32 arg3;
} Item;

Item *_rpc_get_packet(void *);
s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 WaitSema(s32);

s32 sceSifGetOtherData(sceSifReceiveData *c, s32 arg1, s32 arg2, s32 arg3, u32 flags) {
    SemaParam sp;
    Item *it;

    it = _rpc_get_packet(&_sceSifRpcData);
    if (it == NULL) {
        return -1;
    }
    c->rpcd.paddr = it;
    c->rpcd.pid = it->id;
    it->arg = arg1;
    it->arg2 = arg2;
    it->arg3 = arg3;
    it->self = it;
    it->client = c;
    if (!(flags & 1)) {
        sp.maxCount = 1;
        sp.initCount = 0;
        if ((c->rpcd.tid = CreateSema(&sp)) < 0) {
            _rpc_free_packet(it);
            return -3;
        }
        if (sceSifSendCmd(0x8000000C, it, 0x40, 0, 0, 0) == 0) {
            _rpc_free_packet(it);
            DeleteSema(c->rpcd.tid);
            return -2;
        }
        WaitSema(c->rpcd.tid);
        DeleteSema(c->rpcd.tid);
        return 0;
    } else {
        c->rpcd.tid = -1;
        if (sceSifSendCmd(0x8000000C, it, 0x40, 0, 0, 0) == 0) {
            _rpc_free_packet(it);
            return -2;
        }
        return 0;
    }
}
