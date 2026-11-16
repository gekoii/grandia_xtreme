/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "protos/main.h"



typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    void *unk10;
} Pkt;

s32 AddDmacHandler(s32, void *, s32);
void FlushCache(s32);
void sceSifSetDChain(void);
s32 sceSifSetReg(u32, u32);
u32 sceSifGetReg(u32);
s32 EnableDmac(s32);
void DIntr(void);
void EIntr(void);
void _sceSifCmdSetSreg(void);
void _sceSifCmdIntrHandler(void);

void sceSifInitCmd(void) {
    s32 i;
    u32 v;
    Pkt *p;

    DIntr();
    if (_sceSifCmdInitDone != 0) {
        EIntr();
        return;
    }
    _sceSifCmdData.pktbuf = (void *)((u32)_sceSifCmdPktBuf | 0x20000000);
    _sceSifCmdInitDone = 1;
    _sceSifCmdData.cmdbuf = (void *)((u32)_sceSifCmdRecvBuf | 0x20000000);
    _sceSifCmdData.nr_sys_handlers = 32;
    _sceSifCmdData.iopbuf = 0;
    _sceSifCmdData.sys_cmd_handlers = _sceSifSysCmdHandlers;
    _sceSifCmdData.usr_cmd_handlers = 0;
    _sceSifCmdData.nr_usr_handlers = 0;
    _sceSifCmdData.sregs = _sceSifSregs;
    for (i = 0; i < 32; i++) {
        _sceSifSysCmdHandlers[i].handler = 0;
        _sceSifSysCmdHandlers[i].harg = 0;
    }
    for (i = 31; i >= 0; i--) {
        _sceSifSregs[i] = 0;
    }
    _sceSifSysCmdHandlers[0].handler = _sceSifCmdChangeAddr;
    _sceSifSysCmdHandlers[0].harg = &_sceSifCmdData;
    _sceSifSysCmdHandlers[1].handler = _sceSifCmdSetSreg;
    _sceSifSysCmdHandlers[1].harg = &_sceSifCmdData;
    i = 32;
    EIntr();
    FlushCache(0);
    if (*(volatile u32 *)0x1000E010 & 0x20) {
        *(volatile u32 *)0x1000E010 = i;
    }
    if (!(*(volatile u32 *)0x1000C000 & 0x100)) {
        sceSifSetDChain();
    }
    _sceSifCmdDmacHandlerId = AddDmacHandler(5, _sceSifCmdIntrHandler, 0);
    EnableDmac(5);
    v = sceSifGetReg(0x80000000);
    _sceSifCmdData.iopbuf = (void *)v;
    if (v != 0) {
        _sceSifCmdInitPkt.buf = _sceSifCmdPktBuf;
        sceSifSendCmd(0x80000000, &_sceSifCmdInitPkt, 0x14, 0, 0, 0);
        return;
    }
    while (!(sceSifGetReg(4) & 0x20000)) {
    }
    v = sceSifGetReg(2);
    _sceSifCmdData.iopbuf = (void *)v;
    sceSifSetReg(0x80000000, v);
    sceSifSetReg(0x80000001, (u32)&_sceSifCmdData);
    _sceSifCmdInitPkt.buf = _sceSifCmdPktBuf;
    _sceSifCmdInitPkt.hdr.opt = 0;
    sceSifSendCmd(0x80000002, &_sceSifCmdInitPkt, 0x14, 0, 0, 0);
}
