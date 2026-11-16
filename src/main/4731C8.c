/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"

typedef struct {
    u64 tag;
    u64 pad;
} Tag;


void _sceMpegSendBsChain(u32 addr, s32 size) {
    Tag *tag;
    s32 n;
    s32 id;
    u32 hi;

    tag = (Tag *)(((u32)_sceMpegIpuToTags & 0x0FFFFFFF) | 0x20000000);
    while (size > 0) {
        n = (size > 0xFFF40) ? 0xFFF40 : size;
        hi = addr & 0x0FFFFFFF;
        id = 0;
        size -= n;
        if (size != 0) {
            id = 3;
        }
        tag->tag = ((u64)hi << 32) | ((u64)id << 28) | (u32)((n + 15) / 16);
        addr += n;
        tag++;
    }
    *(volatile u32 *)0x1000B430 = (u32)_sceMpegIpuToTags & 0x0FFFFFFF;
    *(volatile u32 *)0x1000B420 = 0;
    *(volatile u32 *)0x1000B400 = 0x105;
}
