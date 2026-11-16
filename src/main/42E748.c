#include "common.h"
#include "main/n03.h"
#include "protos/main.h"

void GsRenderStatePacketSet(void *, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32, s64);
void GsTexPacketSet(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void DbgTextInit(s32 arg0) {
    GsRenderStatePacketSet(dbgTextGsState, 0, 1, 0, 1, 0, 1, 0,
                  0, 1, 6, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1,
                  0, 0, 0, 0, 0, 0, 0, 0, 0);
    GsTexPacketSet(dbgFontTexPkt, arg0, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    DbgTextReset(0, 0, 0, 0);
    DbgTextSetColor(0, 0x80808080);
    DbgTextSetColor(1, 0x80A04040);
    DbgTextSetColor(2, 0x804040A0);
    DbgTextSetColor(3, 0x80A040A0);
    DbgTextSetColor(4, 0x80585858);
    DbgTextSetColor(5, 0x80A0A040);
    DbgTextSetColor(6, 0x8040A0A0);
    DbgTextSetColor(7, 0x80808080);
    DbgTextSetColor(8, 0x804040FF);
}
