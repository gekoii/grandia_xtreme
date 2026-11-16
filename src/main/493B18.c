/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"

void sceSifStopDma(void);
s32 sceSifGetReg(u32);
void sceSifWriteBackDCache(void *, s32);
s32 sceSifSetReg(s32, s32);

s32 sceSifResetIop(char *arg0, s32 arg1) {
    sceSifDmaData dma;
    s32 addr;
    s32 i;

    sceSifStopDma();
    addr = sceSifGetReg(0x80000000);
    _sceSifResetCmd.arg = arg1;
    for (i = 0; arg0[i] != 0; i++) {
        _sceSifResetCmd.name[i] = arg0[i];
    }
    _sceSifResetCmd.dest = NULL;
    _sceSifResetCmd.size = i;
    _sceSifResetCmd.fcode = 0x80000003;
    _sceSifResetCmd.dsize = 0;
    _sceSifResetCmd.psize = 0x68;
    dma.data = (u32)&_sceSifResetCmd;
    dma.addr = addr;
    dma.size = 0x68;
    dma.mode = 0x44;
    sceSifWriteBackDCache(&_sceSifResetCmd, 0x68);
    sceSifSetReg(4, 0x40000);
    if (sceSifSetDma(&dma, 1) != 0) {
        sceSifSetReg(4, 0x10000);
        sceSifSetReg(4, 0x20000);
        sceSifSetReg(0x80000002, 0);
        sceSifSetReg(0x80000000, 0);
        return 1;
    }
    return 0;
}
