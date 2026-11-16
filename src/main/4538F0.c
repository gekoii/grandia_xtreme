#include "common.h"
#include "protos/main.h"
#include "chr.h"


ChrPack *FilePacCharRelocate(ChrPack *h) {
    u8 flags[32];
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 32; i++) {
        flags[i] = 1;
    }
    /* volatile load and store of modelPac: allocation and schedule of the prologue */
    k = *(volatile s32 *)&h->modelPac;
    if (k) {
        k += (s32)h;
        *(volatile s32 *)&h->modelPac = k;
        FilePacModelRelocate(k);
    }
    for (i = 0; i < 8; i++) {
        if (h->items[i]) {
            h->items[i] += (s32)h;
        }
    }
    for (i = 0; i < 32; i++) {
        if (h->motions[i]) {
            for (j = i + 1; j < 32; j++) {
                if (h->motions[j] && h->motions[i] == h->motions[j]) {
                    flags[j] = 0;
                }
            }
        }
    }
    for (i = 0; i < 32; i++) {
        if (h->motions[i]) {
            h->motions[i] += (s32)h;
            if (flags[i]) {
                FileChunkRelocate(h->motions[i]);
            }
            h->motions[i] += 0x10;
        }
        if (h->motionEvents[i]) {
            h->motionEvents[i] += (s32)h;
        }
    }
    if (h->enemyData) {
        h->enemyData += (s32)h;
    }
    for (i = 0; i < 4; i++) {
        if (h->unk134[i]) {
            h->unk134[i] += (s32)h;
        }
    }
    for (i = 0; i < 8; i++) {
        if (h->modelPacks[i]) {
            h->modelPacks[i] += (s32)h;
            FilePacModelRelocate(h->modelPacks[i]);
        }
        if (h->animSets[i]) {
            h->animSets[i] += (s32)h;
            GphAnimSetRelocate(h->animSets[i]);
        }
        if (h->chunks[i]) {
            h->chunks[i] += (s32)h;
            FileChunkRelocate(h->chunks[i]);
            h->chunks[i] += 0x10;
        }
        if (h->unk1A4[i]) {
            h->unk1A4[i] += (s32)h;
        }
    }
    return h;
}
