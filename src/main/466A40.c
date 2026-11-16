#include "common.h"
#include "protos/main.h"
#include "snd.h"

void WaitSema(s32);
void SignalSema(s32);

#define REG(a) (*(volatile s32 *)(a))

s32 viBufStopDMA(ViBuf *arg0) {
    WaitSema(arg0->sema);
    arg0->isActive = 0;
    REG(0x1000F590) = 0x10000;
    setD4_CHCR(5);
    REG(0x1000F590) = 0;
    __asm__ volatile("sync");
    arg0->env[0] = REG(0x1000B410);
    arg0->env[1] = REG(0x1000B430);
    arg0->env[2] = REG(0x1000B420);
    arg0->env[3] = REG(0x1000B400);
    while (REG(0x10002010) & 0xF0) {
    }
    REG(0x1000F590) = 0x10000;
    setD3_CHCR(0);
    REG(0x1000F590) = 0;
    __asm__ volatile("sync");
    arg0->env[4] = REG(0x1000B010);
    arg0->env[5] = REG(0x1000B020);
    arg0->env[6] = REG(0x1000B000);
    arg0->env[7] = REG(0x10002020);
    arg0->env[8] = REG(0x10002010);
    SignalSema(arg0->sema);
    return 1;
}
