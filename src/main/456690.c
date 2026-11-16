#include "common.h"
#include "main/n08.h"

typedef struct {
    GsPacketHead head;
    u64 d30;
    u64 d38;
    u64 d40;
    u64 d48;
    u64 d50;
    u64 d58;
    u64 d60;
    u64 d68;
} Pkt;

void GsMiscPacketInit(Pkt *p) {
    p->head.dmaTag = 0x10000006;
    p->head.id = (u64)0xEEEE << 16;
    p->head.vif[3] = 0x50000005;
    p->head.gifTag[0] = 0x1000000000008004;
    p->head.gifTag[1] = 0xE;
    p->d38 = 0x46;
    p->d30 = 1;
    p->d48 = 0x1A;
    p->d58 = 0x22;
    p->d68 = 0x49;
    p->head.vif[0] = 0;
    p->head.vif[1] = 0;
    p->head.vif[2] = 0;
    p->d40 = 0;
    p->d50 = 0;
    p->d60 = 0;
    __asm__ volatile("sync");
}
