#include "common.h"
#include "main/n07.h"

void GsMfifoSend(void *, s32);

void GsDrawEnvPacketSend(u8 *arg0) {
    *(u64 *)(arg0 + 0x30) = (*(u64 *)(arg0 + 0x30) & ~0xFFFFULL) | (u32)(gsDrawFrameAddr >> 11);
    *(u64 *)(arg0 + 0x40) = (*(u64 *)(arg0 + 0x40) & ~0xFFFFULL) | (u32)(gsZBufAddr >> 11);
    __asm__ volatile("sync");
    GsMfifoSend(arg0, 8);
}
