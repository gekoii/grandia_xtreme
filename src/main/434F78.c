#include "common.h"
#include "gph.h"

void GphMotCursorInit(GphMotCursor *arg0, GphMotion *arg1, f32 arg2) {
    arg0->flags = arg1->flags;
    arg0->frame = arg2;
    arg0->frameCount = arg1->frameCount;
    arg0->trackStart = arg0->track = (s32 *)arg1->unk0;
    arg0->part = 0;
    arg0->unk10 = 0;
    arg0->unkC = arg1->unkA & 0xF;
    arg0->motion = arg1;
}
