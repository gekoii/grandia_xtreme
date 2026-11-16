#include "common.h"
#include "save.h"
#include "protos/main.h"

/* The checksum covers the image from headParty (+0x10) on. */
void McSaveSetChecksum(void *arg0, s32 arg1) {
    SysSaveData *p = arg0;

    p->checksum = McChecksum(p->headParty, arg1 - 0x10);
    p->size = 0x32F0;
}

s32 McSaveCheckChecksum(void *arg0) {
    SysSaveData *p = arg0;
    u32 sum;

    sum = p->checksum;
    return sum == McChecksum(p->headParty, p->size);
}
