/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "protos/main.h"

typedef struct {
    u8 pad0[0x10];
    s32 rpc_id;
    s32 pad14;
    s32 rec_id;
    void *client;
    s32 cid;
} Rend;


void sceSifWriteBackDCache(void *, s32);
s32 DIntr(void);
s32 EIntr(void);
Rend *_rpc_get_fpacket2(void *, s32);
Rend *_rpc_get_fpacket(void *);

void sceSifExecRequest(sceSifServeData *sd) {
    sceSifDmaData dmat[2];
    Rend *rend;
    void *rec;
    s32 len;
    s32 count;
    s32 id;
    s32 i;

    len = 0;
    rec = sd->func(sd->fno, sd->buff, sd->size);
    if (rec != NULL) {
        len = sd->rsize;
    }
    if (sd->size > 0) {
        sceSifWriteBackDCache(sd->buff, sd->size);
    }
    if (len > 0) {
        sceSifWriteBackDCache(rec, len);
    }
    DIntr();
    if (sd->rid & 4) {
        rend = _rpc_get_fpacket2(&_sceSifRpcData, sd->rid >> 16);
    } else {
        rend = _rpc_get_fpacket(&_sceSifRpcData);
    }
    EIntr();
    rend->cid = 0x8000000A;
    rend->client = sd->client;
    if (sd->rmode != 0) {
        while (sceSifSendCmd(0x80000008, rend, 0x40, rec, (s32)sd->receive, len) == 0) {
        }
        return;
    }
    rend->rec_id = 0;
    rend->rpc_id = 0;
    count = 0;
    if (len > 0) {
        count = 1;
        dmat[0].data = (u32)rec;
        dmat[0].addr = (u32)sd->receive;
        dmat[0].size = len;
        dmat[0].mode = 0;
    }
    dmat[count].data = (u32)rend;
    dmat[count].addr = (u32)sd->paddr;
    dmat[count].size = 0x40;
    dmat[count].mode = 0;
    count++;
    do {
        id = sceSifSetDma(dmat, count);
        if (id == 0) {
            for (i = 0x100000; i != -1; i--) {
            }
        }
    } while (id == 0);
}
