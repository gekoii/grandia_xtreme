#include "common.h"
#include "party.h"
#include "main/n01.h"

s32 ChrPortraitExists(u32 arg0) {
    ChrPortraitIndex *t = chrPortraitIndexTable; if (arg0 >= t->count) { return 0; } return t->entries[arg0].faceIndex != 0xFFFF;
}
