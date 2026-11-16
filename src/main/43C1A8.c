#include "common.h"
#include "main/n05.h"

void MemSet(void *, s32, s32);

void GphEnvMapInit(void) {
    MemSet(gphEnvMapTable, 0, 0xC00);
    gphEnvMapGsPkt.dmaTag = 0x10000006;
    gphEnvMapGsPkt.direct = 0x50000005;
    gphEnvMapGsPkt.gifTag = 0x1000400000008004;
    gphEnvMapGsPkt.gifRegs = 0xE;
    gphEnvMapGsPkt.reg[0].data = 0x50;
    gphEnvMapGsPkt.reg[0].addr = 0x1B;
    gphEnvMapGsPkt.reg[1].data = 0x61;
    gphEnvMapGsPkt.reg[1].addr = 0x14;
    gphEnvMapGsPkt.reg[2].data = 0x7140D;
    gphEnvMapGsPkt.reg[2].addr = 0x47;
    gphEnvMapGsPkt.reg[3].data = 0x44;
    gphEnvMapGsPkt.dmaTagHi = 0;
    gphEnvMapGsPkt.vif[0] = 0;
    gphEnvMapGsPkt.vif[1] = 0;
    gphEnvMapGsPkt.vif[2] = 0;
    gphEnvMapGsPkt.reg[3].addr = 0x42;
}
