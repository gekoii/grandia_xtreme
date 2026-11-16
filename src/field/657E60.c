#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "field/n25.h"


void FldFlagListSetPage(u8 page) {
    s32 base;
    s32 n;
    s32 i;

    base = page * 6;
    n = fldFlagListWork->count - base;
    if (n >= 6) {
        n = 6;
    }
    TxtGlyphQueueRequestClear(0);
    for (i = 0; i < n; i++) {
        TxtGlyphQueueMsg(0, sysGrxsysPac->msg->tbl[1], i * 2, fldFlagListWork->entries[base + i]);
    }
}
