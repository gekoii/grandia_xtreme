#include "common.h"
#include "main/n08.h"

typedef struct {
    GsPacketHead head;
    u64 prmode;
    u64 prmodeReg;
    u64 test;
    u64 testReg;
    u64 alpha;
    u64 alphaReg;
    u64 fba;
    u64 fbaReg;
    u64 tex1;
    u64 tex1Reg;
    u64 texa;
    u64 texaReg;
} RenderStatePacket;

void GsRenderStatePacketSet(RenderStatePacket *p, u32 iip, u32 tme, u32 fge, u32 abe, u32 aa1, u32 fst, u32 ctxt, u32 fix, u32 ate, u32 atst, u32 aref, u32 afail, u32 date, u32 datm, u32 zte, u32 ztst, u32 a, u32 b, u32 c, u32 d, u32 afix, u32 fba, u32 lcm, u32 mxl, u32 mmag, u32 mmin, u32 l, s32 k, u64 texa) {
    p->head.dmaTag = 0x10000008;
    p->head.id = 0xEEEE0003;
    p->head.vif[3] = 0x50000007;
    p->head.gifTag[0] = 0x1000000000008006;
    p->head.gifTag[1] = 0xE;
    p->prmode = ((u64)iip << 3) + ((u64)tme << 4) + ((u64)fge << 5) + ((u64)abe << 6) + ((u64)aa1 << 7) + ((u64)fst << 8) + ((u64)ctxt << 9) + ((u64)fix << 10);
    p->prmodeReg = 0x1B;
    p->test = (u64)ate + ((u64)atst << 1) + ((u64)aref << 4) + ((u64)afail << 12) + ((u64)date << 14) + ((u64)datm << 15) + ((u64)zte << 16) + ((u64)ztst << 17);
    p->testReg = 0x47;
    p->alpha = (u64)a + ((u64)b << 2) + ((u64)c << 4) + ((u64)d << 6) + ((u64)afix << 32);
    p->alphaReg = 0x42;
    p->fba = fba;
    p->fbaReg = 0x4A;
    p->head.vif[0] = 0;
    p->head.vif[1] = 0;
    p->head.vif[2] = 0;
    p->tex1 = (u64)lcm + ((u64)mxl << 2) + ((u64)mmag << 5) + ((u64)mmin << 6) + ((u64)l << 15) + ((u64)k << 32);
    p->tex1Reg = 0x14;
    p->texa = texa;
    p->texaReg = 0x3B;
    __asm__ volatile("sync");
}
