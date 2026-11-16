#include "common.h"
#include "party.h"

extern u8 itemFieldUseKindTable[];

s32 ItemCanUseInField(s16 arg0) {
    s16 e;
    PtyItem *p;
    s8 k;

    e = ptyPartyData->bag[arg0].id;
    if (e == 0) {
        return -1;
    }
    p = (PtyItem *)((e << 5) + (u32)ptyItemTable);
    if (p->type == 0xD) {
        return -1;
    }
    k = p->use->effects->kind;
    if (k >= 8) {
        return -1;
    }
    return itemFieldUseKindTable[k] ? 0 : -1;
}
