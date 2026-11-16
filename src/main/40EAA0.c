#include "common.h"
#include "party.h"
#include "main/n01.h"

s32 ChrPortraitGetName(u32 arg0) {
    ChrPortraitIndex *t = chrPortraitIndexTable;
    u16 idx;
    if (arg0 >= t->count) return 0;
    idx = t->entries[arg0].nameIndex;
    if (idx >= t->nameCount) return 0;
    return t->names[idx];
}
