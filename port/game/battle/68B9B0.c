/* Port-only C for FxTraceStripDraw (0x0068B9B0), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/main.h"
#include "protos/battle.h"
#include "gph.h"
#include "battle/misc.h"

/* One edge of the strip, newest point pair first: corners k (outer, the
 * pair pushed out by up to 0.6 past b) and k + 1 (on the pair), coloured
 * from c1 (old) to c0 (new) by f = i / (unk10 - 1) and f * f, u from the
 * strip's age */
static void FxTraceStripEdge(GphQuad3D *q, FxTraceStrip *t, s32 k, s32 i, s32 j, s32 uMax, s32 rev)
{
    GphQuad3DCol *c = &q->col[k];
    FxTraceOfs *s;
    f32 f;
    f32 g;
    f32 h;
    f32 u;

    f = (f32)i / (f32)(t->unk10 - 1);
    u = rev ? (f32)uMax * f : (f32)uMax * (1.0f - f);
    q->uv[k + 1][0] = (s32)u;
    q->uv[k][0] = q->uv[k + 1][0];

    c[0].r = (s32)(t->c0.b[3] * f + t->c1.b[3] * (1.0f - f));
    c[0].g = (s32)(t->c0.b[2] * f + t->c1.b[2] * (1.0f - f));
    c[0].b = (s32)(t->c0.b[1] * f + t->c1.b[1] * (1.0f - f));
    c[0].a = (s32)(t->c0.b[0] * f + t->c1.b[0] * (1.0f - f));
    f = f * f;
    c[1].r = (s32)(t->c0.b[3] * f + t->c1.b[3] * (1.0f - f));
    c[1].g = (s32)(t->c0.b[2] * f + t->c1.b[2] * (1.0f - f));
    c[1].b = (s32)(t->c0.b[1] * f + t->c1.b[1] * (1.0f - f));
    c[1].a = (s32)(t->c0.b[0] * f + t->c1.b[0] * (1.0f - f));

    s = &t->buf[j];
    g = 1.0f - (f32)i / (f32)(t->unk10 - 1);
    h = g * 0.6f + 1.0f;
    q->pos[k].x = s->a[0] + (s->b[0] - s->a[0]) * h;
    q->pos[k].y = s->a[1] + (s->b[1] - s->a[1]) * h;
    q->pos[k].z = s->a[2] + (s->b[2] - s->a[2]) * h;
    q->pos[k].w = 0.0f;
    q->pos[k + 1].x = s->a[0] + (s->b[0] - s->a[0]) * g;
    q->pos[k + 1].y = s->a[1] + (s->b[1] - s->a[1]) * g;
    q->pos[k + 1].z = s->a[2] + (s->b[2] - s->a[2]) * g;
    q->pos[k + 1].w = 0.0f;
}

/* Draws a trace strip as quads between consecutive point pairs of its ring
 * buffer, newest (cur) first, one GphBatchDrawQuads3D per quad */
void FxTraceStripDraw(FxTraceStrip *t)
{
    GphQuad3D q[2];
    s32 rev;
    s32 w;
    s32 i;
    s32 j;
    s32 k;

    if (t->unk10 < 2) {
        return;
    }
    switch ((u8)t->unk16) {
    case 1:
    case 3:
        GphTexBind(0x3F);
        rev = 0;
        w = 6;
        break;
    case 2:
        GphTexBind(0x40);
        rev = 0;
        w = 3;
        break;
    case 6:
        GphTexBind(0x40);
        rev = 0;
        w = 5;
        break;
    case 4:
    case 8:
        GphTexBind(0x3F);
        rev = 1;
        w = 4;
        break;
    case 5:
    case 7:
    case 10:
        GphTexBind(0x3E);
        rev = 0;
        w = 6;
        break;
    default:
        GphTexBind(0x35);
        rev = 0;
        w = 1;
        break;
    }

    q[0].type = ((u8)t->unk15 & 3) | 0xC;
    for (k = 0; k < 4; k++) {
        q[0].col[k].r = 0xFF;
        q[0].col[k].g = 0;
        q[0].col[k].b = 0;
        q[0].col[k].a = 0x80;
    }
    q[1].type = 0; /* end of the list */
    q[0].uv[0][1] = 0;
    q[0].uv[1][1] = 0x1F;
    q[0].uv[2][1] = 0;
    q[0].uv[3][1] = 0x1F;

    i = t->unk10 - 1;
    j = t->cur;
    FxTraceStripEdge(q, t, 0, i, j, (w << 5) + 1, rev);
    if (--j < 0) {
        j = t->count - 1;
    }
    for (i--, k = 2; i >= 0; i--, k ^= 2) {
        FxTraceStripEdge(q, t, k, i, j, (w << 5) + 1, rev);
        GphBatchDrawQuads3D(q);
        if (--j < 0) {
            j = t->count - 1;
        }
    }
}
