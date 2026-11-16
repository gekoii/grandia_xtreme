/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "sdk/eekernel.h"
#include "protos/main.h"

typedef struct Client Client;
typedef struct Call {
    u8 pad0[0x14];
    s32 self;
    void *id;
    s32 client;
    s32 rpc_number;
    s32 ssize;
    s32 recv;
    s32 rsize;
    s32 rmode;
    void *server;
} Call;

struct Client {
    union { Call *p; s32 i; } call;
    s32 id;
    s32 sema;
    s32 xC;
    s32 x10;
    s32 buff;
    s32 x18;
    s32 endfunc;
    union { void *p; s32 i; } endparam;
    s32 server;
};

Call *_rpc_get_packet(void *);
s32 CreateSema(SemaParam *);
s32 DeleteSema(s32);
s32 WaitSema(s32);
void sceSifWriteBackDCache(void *, s32);

s32 sceSifCallRpc(Client *c, s32 rpc_number, u32 mode, void *send, s32 ssize, void *recv, s32 rsize, void *endfunc, void *endparam) {
    SemaParam sp;
    Call *call;
    s32 id;

    call = _rpc_get_packet(&_sceSifRpcData);
    if (call == NULL) {
        return -1;
    }
    id = (s32)call->id;
    c->call.p = call;
    c->id = id;
    c->endfunc = (s32)endfunc;
    c->endparam.p = endparam;
    call->rpc_number = rpc_number;
    call->ssize = ssize;
    call->recv = (s32)recv;
    call->rsize = rsize;
    call->self = (s32)call;
    call->server = (void *)c->server;
    call->client = (s32)c;
    if (!(mode & 2)) {
        if (send == recv) {
            sceSifWriteBackDCache(send, ssize < rsize ? rsize : ssize);
        } else {
            if (ssize > 0) {
                sceSifWriteBackDCache(send, ssize);
            }
            if (rsize > 0) {
                sceSifWriteBackDCache(recv, rsize);
            }
        }
    }
    if (mode & 1) {
        if (endfunc == NULL) {
            call->rmode = 0;
        } else {
            call->rmode = 1;
        }
        c->sema = -1;
        if (sceSifSendCmd(0x8000000A, call, 0x40, send, c->buff, ssize) != 0) {
            return 0;
        }
        _rpc_free_packet(call);
        return -2;
    }
    sp.maxCount = 1;
    sp.initCount = 0;
    if ((c->sema = CreateSema(&sp)) < 0) {
        _rpc_free_packet(call);
        return -3;
    }
    call->rmode = 1;
    if (sceSifSendCmd(0x8000000A, call, 0x40, send, c->buff, ssize) == 0) {
        DeleteSema(c->sema);
        _rpc_free_packet(call);
        return -2;
    }
    WaitSema(c->sema);
    DeleteSema(c->sema);
    return 0;
}
