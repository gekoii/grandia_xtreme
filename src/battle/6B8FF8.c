#include "common.h"
#include "battle/n32.h"

void *BtlPakGetEntry(BtlPak *pak, s32 i) {
    s32 off;

    if (i >= pak->count || (off = ((BtlPak *)(i * 8 + (u32)pak))->e[0].offset) == 0) {
        return NULL;
    }
    return (u8 *)pak + off;
}
