#include "common.h"
#include "protos/main.h"
#include "ptcl.h"

extern u8 ptclDataFileName[];

void DbgPtclSaveFile(void) {
    PtclPtbHeader *h;
    u8 *dst;
    s32 size;
    s32 i;
    s32 j;
    PtclCtrl *src;
    PtclCtrl *d;
    s32 k;
    PtclGenDef *sub;
    PtclEffector *it;
    PtclStarterEntry *rec;
    u8 *s;

    h = MemCalloc16C(0x10000);
    size = 0x20;
    h->ctrlOffset = 0x20;
    dst = (u8 *)(h + 1);
    src = (PtclCtrl *)((u8 *)ptclPtbData + ptclPtbData->ctrlOffset);
    for (i = 0; i < ptclPtbData->ctrlCount; i++) {
        d = (PtclCtrl *)dst;
        *d = *src++;
        dst += sizeof(PtclCtrl);
        size += sizeof(PtclCtrl);
        for (j = 0, sub = d->keys; j < 4; j++, sub++) {
            for (k = 0; k < 4; k++) {
                if (sub->effectors[k] >= 0 && sub->effectors[k] < ptclMainPool->effectorCount) {
                    sub->effectors[k] = ptclMainPool->effectors[sub->effectors[k]].id;
                }
            }
        }
    }
    h->ctrlCount = ptclPtbData->ctrlCount;
    h->effectorOffset = size;
    it = ptclMainPool->effectors;
    for (i = 0; i < ptclMainPool->effectorCount; i++, it++) {
        if (it->id >= 0) {
            *(PtclEffector *)dst = *it;
            size += sizeof(PtclEffector);
            dst += sizeof(PtclEffector);
            h->effectorCount++;
        }
    }
    h->starterOffset = size;
    rec = ptclStarterTable;
    for (j = 0; j < 16; j++, rec++) {
        if (rec->time >= 0) {
            *(PtclStarterEntry *)dst = *rec;
            size += sizeof(PtclStarterEntry);
            dst += sizeof(PtclStarterEntry);
            h->starterCount++;
        }
    }
    h->stringOffset = size;
    h->stringCount = ptclPtbData->stringCount;
    s = (u8 *)ptclPtbData + ptclPtbData->stringOffset;
    for (k = 0; k < ptclPtbData->stringCount; k++) {
        while (*s != 0) {
            *dst++ = *s++;
            size++;
        }
        *dst++ = *s++;
        size++;
    }
    FileWrite(ptclDataFileName, h, size);
    MemFree(h);
}
