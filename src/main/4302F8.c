#include "common.h"
#include "gph.h"

void GsTexPacketSet(GsTexPacket *, GphTex *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void FlushCache(s32);

void GsTexLoadPacket(GsTexPacket *a0, GphTex *a1, s32 a2, s32 a3, s32 t0, s32 t1, s32 t2, s32 t3, s32 s0, s32 s1, s32 s2, s32 s3) {
    GsTexPacketSet(a0, a1, a2, a3, t0, t1, t2, t3, s0, s1, s2, s3);
    FlushCache(0);
}
