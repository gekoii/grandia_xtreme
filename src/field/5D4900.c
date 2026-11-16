/* as: ee-gcc2.96-hazard */
/* cc: ee-gcc2.9-991111-01 */
/* rodata: 0x006714E8-0x006714F0 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "grxsys.h"
#include "field/state.h"

typedef struct {
    u8 pad0[0xC];
    u8 *str;
    u8 pad10[0x10];
} Msg;




static inline FldTreasure *ent(s32 i) {
    return &fldTreasureWork[i];
}

void FldTreasureDrawNames(void) {
    GphBatchQuad spr[140];
    s8 offs[4][2] = { { 2, 2 }, { -2, -2 }, { 2, -2 }, { -2, 2 } };
    s32 codes[32];
    Msg msg;
    s8 c0;
    s8 c1;
    GphBatchQuad *p;
    FldTreasure *e;
    s32 i;
    s32 n;
    s32 k;
    s32 found;
    s32 width;
    f32 hw;
    f32 fx;
    f32 fy;
    f32 fh;
    f32 scale;
    f32 cw;

    GphBatchSetOffset(0, 0);
    if (camCtl.aiType == 2) {
        return;
    }
    found = 0;
    for (i = 0; i < 64; i++) {
        if (ent(i)->state == 2) {
            found = 1;
            break;
        }
    }
    if (found == 0) {
        return;
    }
    TxtWinTextureBind(2);
    for (i = 0; i < 64; i++) {
        e = ent(i);
        if (e->state != 2) continue;
        if (e->t < 0.1f) continue;
        p = spr;
        if (e->namePos[2] < 0.0f) continue;
        hw = e->nameH * 0.5f;
        if (e->namePos[1] + hw < 0.0f) continue;
        if (e->namePos[1] - hw > 448.0f) continue;
        hw = e->nameW * 0.5f;
        if (e->namePos[0] + hw < 0.0f) continue;
        if (e->namePos[0] - hw > 640.0f) continue;
        ItemInfoFill(&msg, e->item, 0);
        TxtMsgGetCodes(msg.str, -1, codes);
        width = TxtStrGetWidth(msg.str, 12);
        e = ent(i);
        fh = 2.0f * e->nameH;
        scale = 2.0f * e->nameW / (f32)width;
        fx = e->namePos[0] - e->nameW;
        fy = e->namePos[1] - e->nameH;
        for (n = 0; n < ent(i)->nameLen; n++) {
            TxtGlyphGetMetrics(codes[n], (u8 *)&c0, (u8 *)&c1, sysGrxsysPac->glyphTable);
            cw = (f32)(c0 - c1) * scale;
            for (k = 0; k < 4; k++) {
                p->type = 1;
                p->v[0].r = 0x20;
                p->v[0].g = 0x40;
                p->v[0].b = 0x20;
                p->v[0].a = 0x60;
                p->v[1].r = 0x20;
                p->v[1].g = 0x40;
                p->v[1].b = 0x20;
                p->v[1].a = 0x60;
                p->v[2].r = 0x20;
                p->v[2].g = 0x40;
                p->v[2].b = 0x20;
                p->v[2].a = 0x60;
                p->v[3].r = 0x20;
                p->v[3].g = 0x40;
                p->v[3].b = 0x20;
                p->v[3].a = 0x60;
                p->v[0].u = c1 + n * 18;
                p->v[0].v = ent(i)->nameRow * 18;
                p->v[1].u = c1 + n * 18;
                p->v[1].v = ent(i)->nameRow * 18 + 16;
                p->v[2].u = c1 + n * 18 + (c0 - c1);
                p->v[2].v = ent(i)->nameRow * 18;
                p->v[3].u = c1 + n * 18 + (c0 - c1);
                p->v[3].v = ent(i)->nameRow * 18 + 16;
                p->v[0].x = offs[k][0] + (s32)fx;
                p->v[0].y = offs[k][1] + (s32)fy;
                p->v[1].x = offs[k][0] + (s32)fx;
                p->v[1].y = offs[k][1] + (s32)fy + (s32)fh;
                p->v[2].x = offs[k][0] + (s32)fx + (s32)cw;
                p->v[2].y = offs[k][1] + (s32)fy;
                p->v[3].x = offs[k][0] + (s32)fx + (s32)cw;
                p->v[3].y = offs[k][1] + (s32)fy + (s32)fh;
                p->z = 0;
                p++;
            }
            p->type = 1;
            p->v[0].r = 0x80;
            p->v[0].g = 0x80;
            p->v[0].b = 0x80;
            p->v[0].a = 0x80;
            p->v[1].r = 0x80;
            p->v[1].g = 0x80;
            p->v[1].b = 0x80;
            p->v[1].a = 0x80;
            p->v[2].r = 0x80;
            p->v[2].g = 0x80;
            p->v[2].b = 0x80;
            p->v[2].a = 0x80;
            p->v[3].r = 0x80;
            p->v[3].g = 0x80;
            p->v[3].b = 0x80;
            p->v[3].a = 0x80;
            p->v[0].u = c1 + n * 18;
            p->v[0].v = ent(i)->nameRow * 18;
            p->v[1].u = c1 + n * 18;
            p->v[1].v = ent(i)->nameRow * 18 + 16;
            p->v[2].u = c1 + n * 18 + (c0 - c1);
            p->v[2].v = ent(i)->nameRow * 18;
            p->v[3].u = c1 + n * 18 + (c0 - c1);
            p->v[3].v = ent(i)->nameRow * 18 + 16;
            p->v[0].x = (s32)fx;
            p->v[0].y = (s32)fy;
            p->v[1].x = (s32)fx;
            p->v[1].y = (s32)fy + fh;
            p->v[2].x = (s32)fx + cw;
            p->v[2].y = (s32)fy;
            p->v[3].x = (s32)fx + cw;
            p->v[3].y = (s32)fy + fh;
            p->z = 0;
            fx += cw + 2.0f;
            p++;
        }
        p->type = 0;
        GphBatchDrawQuads(spr);
    }
}
