#include "common.h"
#include "main/n03.h"
#include "protos/main.h"

void GsTexUpload(void *);

void DbgTextDraw(void) {
    GsTexUpload(dbgFontTexPkt);
    GsTexPacketSend((s32)dbgFontTexPkt);
    GsRenderStatePacketSend(dbgTextGsState);
    DbgTextDrawChars();
}
