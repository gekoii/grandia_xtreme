#include "common.h"
#include "protos/main.h"

s32 GphProjTexCalcUv(void *, void *, s32);
void MemMove128(void *, void *, s32);

s32 GphProjTexAddGaObj(void *arg0, s32 arg1, void *arg2) {
    s32 size;
    s32 a;
    s32 b;
    s32 c;

    MemMove128((void *)0x70002600, arg0, 8);
    if (GphProjTexCalcUv((void *)0x70002600, (void *)0x70003400, arg1) <= 0) {
        return 0;
    }
    size = (*(s32 *)0x70002648 * 8 + 0x97) & 0xFFFFFF80;
    a = GsPacketAlloc64(size);
    b = GsPacketAlloc64(0x80);
    c = GsPacketAlloc64(0x40);
    *(s32 *)0x70002668 = a;
    MemMove128((void *)a, (void *)0x70003400, size >> 4);
    MemMove128((void *)b, (void *)0x70002600, 8);
    MemMove128((void *)c, arg2, 4);
    GphProjTexAddEntry(b, c);
    return 1;
}
