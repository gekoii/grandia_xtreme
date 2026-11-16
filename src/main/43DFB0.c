#include "common.h"
#include "main/n06.h"

void GphTexVertsProject(s32, void *, s32);

void GphTexPrim3DBuild(GphPrimPkt *p, u64 arg1, s32 arg2, s32 arg3) {
    s32 n;

    n = ((arg2 * 3 + 7) & 0xFFFE) >> 1;
    p->dmaTag = 0x10000000 | (n + 1);
    p->vifDirect = n | 0x50000000;
    p->gifTag = (arg1 << 47) | 0x1000400000008001;
    p->gifRegs = 0xE;
    p->reg0 = 0x58;
    p->reg1 = 0x1B;
    p->gifTag2 = (arg2 | 0x8000) | 0x3400000000000000;
    p->gifRegs2 = 0x512;
    p->unk8 = 0;
    p->vif0 = 0;
    p->vif1 = 0;
    p->vif2 = 0;
    GphTexVertsProject(arg2, p->data, arg3);
}
