/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "protos/main.h"


typedef struct {
    s32 f0;
    s32 f4;
    s32 f8;
    s32 fC;
} Pkt;

extern s32 sceSifSetReg(s32, s32);
extern s32 sceSifGetReg(s32);
extern s32 DIntr(void);
extern s32 EIntr(void);
extern void _request_end(void);
extern void _request_rdata(void);
extern void _request_bind(void);
extern void _request_call(void);

void sceSifInitRpc(s32 arg0) {
    Pkt *p;
    u8 *buf;
    sceSifRpcCtrl *c;

    DIntr();
    if (_sceSifRpcInitDone != 0) {
        EIntr();
        return;
    }
    _sceSifRpcInitDone = 1;
    EIntr();
    sceSifInitCmd();
    DIntr();
    buf = (u8 *)_sceSifRpcPktTable;
    c = &_sceSifRpcData;
    c->pkt_table = (sceSifRpcPkt *)((u32)buf | 0x20000000);
    c->pkt_table_len = 0x20;
    c->unused1 = 0;
    c->unused2 = 0;
    c->rdata_table = (sceSifRpcPkt *)((u32)_sceSifRpcRdataTable | 0x20000000);
    c->rdata_table_len = 0x20;
    c->client_table = (sceSifRpcPkt *)((u32)_sceSifRpcClientTable | 0x20000000);
    c->client_table_len = 0x20;
    c->rdata_table_idx = 0;
    _sceSifRpcData.pid = 1;
    sceSifAddCmdHandler(0x80000008, _request_end, c);
    sceSifAddCmdHandler(0x80000009, _request_bind, c);
    sceSifAddCmdHandler(0x8000000A, _request_call, c);
    sceSifAddCmdHandler(0x8000000C, _request_rdata, c);
    EIntr();
    if (sceSifGetReg(0x80000002) != 0) {
        return;
    }
    p = (Pkt *)(buf + 0x40);
    p->fC = 1;
    sceSifSendCmd(0x80000002, p, 0x10, 0, 0, 0);
    do {
    } while (sceSifGetSreg(0) == 0);
    sceSifSetReg(0x80000002, 1);
}
