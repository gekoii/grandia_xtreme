#include "common.h"
#include "protos/field.h"
#include "field/menu.h"


void MemSet(void *, s32, s32);

void FldMenuWinOpen(u8 id, s16 x, s16 y, u16 w, u16 h, u16 unk2, u8 type, s32 flags) {
    FldMenuWin *e = &fldMenuWins[id];
    f32 off;
    s32 i;

    MemSet(e, 0, sizeof(FldMenuWin));
    e->active = 1;
    e->unk34 = 0;
    e->alpha = 0.0f;
    ((u8 *)e)[3] = type; /* +0x3 open type: pad3 in FldMenuWin */
    e->state = 1;
    e->timer = unk2;
    e->h2 = h;
    e->w2 = w;
    e->flags = flags;
    for (i = 15; i >= 0; i--) {
        e->layerKind[i] = 1;
    }
    if (e->flags & 0x4000) {
        e->unk24 = e->h2;
        e->h2 = 0;
    }
    if (type == 8) {
        type = VuRandInt(8);
        off = 64.0f;
    } else {
        off = 128.0f;
    }
    switch (type) {
    case 0:
        e->openStart[0] = (f32)x - off;
        e->openStart[1] = (f32)y;
        e->openStart[2] = 0.0f;
        e->openStart[3] = 0.0f;
        break;
    case 1:
        e->openStart[0] = (f32)x + off;
        e->openStart[1] = (f32)y;
        e->openStart[2] = 0.0f;
        e->openStart[3] = 0.0f;
        break;
    case 2:
        e->openStart[0] = (f32)x;
        e->openStart[1] = (f32)y - off;
        e->openStart[2] = 0.0f;
        e->openStart[3] = 0.0f;
        break;
    case 3:
        e->openStart[0] = (f32)x;
        e->openStart[1] = (f32)y + off;
        e->openStart[2] = 0.0f;
        e->openStart[3] = 0.0f;
        break;
    case 4:
        e->openStart[0] = (f32)x - off;
        e->openStart[1] = (f32)y - off;
        e->openStart[2] = 0.0f;
        e->openStart[3] = 0.0f;
        break;
    case 5:
        e->openStart[0] = (f32)x + off;
        e->openStart[1] = (f32)y - off;
        e->openStart[2] = 0.0f;
        e->openStart[3] = 0.0f;
        break;
    case 6:
        e->openStart[0] = (f32)x - off;
        e->openStart[1] = (f32)y + off;
        e->openStart[2] = 0.0f;
        e->openStart[3] = 0.0f;
        break;
    case 7:
        e->openStart[0] = (f32)x + off;
        e->openStart[1] = (f32)y + off;
        e->openStart[2] = 0.0f;
        e->openStart[3] = 0.0f;
        break;
    case 8:
        break;
    case 9:
        e->state = 4;
        e->alpha = 0.8f;
        e->pos[0] = (f32)x;
        e->pos[1] = (f32)y;
        e->pos[2] = 0.0f;
        e->pos[3] = 0.0f;
        break;
    }
    e->openEnd[0] = (f32)x;
    e->openEnd[1] = (f32)y;
    e->openEnd[2] = 0.0f;
    e->openEnd[3] = 0.0f;
}
