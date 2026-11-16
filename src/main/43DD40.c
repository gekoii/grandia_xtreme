#include "common.h"
#include "protos/main.h"

/* Defined in 43DDA8.c with local types (GphPrimPkt *, u64, s32, Vert *). */
void GphTexPrim2DBuild(s32, s32, s32, s32);

void GphTexPrim2DDraw(s32 arg0, s32 arg1, s32 arg2) {
    s32 pkt;

    pkt = GsPacketAlloc64(arg1 * 0x18 + 0x60);
    GphTexPrim2DBuild(pkt, arg0, arg1, arg2);
    GphPrimListAdd(pkt);
}
