#include "common.h"
#include "battle/n32.h"

s32 BtlPakGetEntrySize(BtlPak *pak, s32 i) {
    if (i >= pak->count) {
        return 0;
    }
    return ((BtlPak *)(i * 8 + (u32)pak))->e[0].size;
}
