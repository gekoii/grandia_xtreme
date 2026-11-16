#include "common.h"
#include "snd.h"
#include "protos/main.h"

void DeleteSema(s32);

s32 viBufDelete(ViBuf *arg0) {
    *(volatile u32 *)0x1000F590 = 0x10000;
    setD4_CHCR(5);
    *(volatile u32 *)0x1000F590 = 0;
    asm volatile("sync");
    *(volatile u32 *)0x1000B420 = 0;
    *(volatile u32 *)0x1000B410 = 0;
    *(volatile u32 *)0x1000B430 = 0;
    DeleteSema(arg0->sema);
    return 1;
}
