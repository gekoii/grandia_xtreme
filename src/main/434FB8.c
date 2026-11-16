#include "common.h"
#include "gph.h"

void GphMotCursorNextPart(GphMotCursor *arg0) {
    u16 *p = (u16 *)arg0->track;
    u16 n = arg0->part;
    u16 m = *p;
    arg0->unk10 = 0;
    arg0->part = n + 1;
    p = (u16 *)((u8 *)p + (m + 1) * 4);
    arg0->track = (s32 *)p;
    arg0->trackStart = (s32 *)p;
}
