/* Port-only C for FldThreatGaugeDraw (0x006020A8), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "vu.h"
#include "field/n21.h"

void GsTexUpload(GsTexPacket *);
void VuBezier3(FVECTOR *, FVECTOR *, f32);
void VuVecLerp(FVECTOR *, FVECTOR *, FVECTOR *, f32);
void GsQuadRotate(GphBatchQuad *, s32, s32, s32, s32, f32);
int grx_ws_hud_side(void);

/* {dx, dy} of the four copies of the eye and the label (signed bytes) */
extern const u8 fldThreatGaugeRodata[0x10];

static void QuadColor(GphBatchQuad *q, s32 r, s32 g, s32 b, s32 a) {
    s32 i;

    for (i = 0; i < 4; i++) {
        q->v[i].r = r;
        q->v[i].g = g;
        q->v[i].b = b;
        q->v[i].a = a;
    }
}

/* Corners 0..3 are (x0,y0) (x0,y1) (x1,y0) (x1,y1); the texels likewise.
   The sort key at +0xC is cleared too. */
static void QuadRect(GphBatchQuad *q, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0, s32 u1, s32 v1) {
    q->v[0].x = x0;
    q->v[0].y = y0;
    q->v[1].x = x0;
    q->v[1].y = y1;
    q->v[2].x = x1;
    q->v[2].y = y0;
    q->v[3].x = x1;
    q->v[3].y = y1;
    q->z = 0;
    q->v[0].u = u0;
    q->v[0].v = v0;
    q->v[1].u = u0;
    q->v[1].v = v1;
    q->v[2].u = u1;
    q->v[2].v = v0;
    q->v[3].u = u1;
    q->v[3].v = v1;
}

void FldThreatGaugeDraw(void) {
    s8 ofs[4][2];
    FVECTOR keys[4];
    FVECTOR out;
    FVECTOR c0;
    FVECTOR c1;
    GphBatchQuad *q;
    s32 x;
    s32 y;
    s32 i;
    s32 a;

    __builtin_memcpy(ofs, fldThreatGaugeRodata, sizeof(ofs));
    if (FldThreatGaugeIsHidden() == 1) {
        return;
    }
    GsTexUpload((GsTexPacket *)fldThreatGauge->texPacket);
    GsTexPacketSend((GsTexPacket *)fldThreatGauge->texPacket);

    /* Slide-in: only x of the keys is set (and only x of the result read) */
    keys[0][0] = 100.0f;
    keys[1][0] = 0.0f;
    keys[2][0] = 0.0f;
    keys[3][0] = 0.0f;
    VuBezier3(&out, keys, fldThreatGauge->fadeIn);
    if (fldThreatGauge->level == 19) {
        x = (s32)VuRandSigned((f32)(fldThreatGauge->vibrateTimer * 3));
        y = (s32)VuRandSigned((f32)(fldThreatGauge->vibrateTimer * 3));
    } else {
        x = 0;
        y = 0;
    }
    /* Widescreen: kept at the right edge of the screen (port/docs/widescreen.md) */
    GphBatchSetOffset(0x190 + grx_ws_hud_side(), (s32)(out[0] + 390.0f));

    /* Frame */
    q = (GphBatchQuad *)&fldThreatGauge->pad34[0x40 - 0x34];
    q->type = 1;
    QuadColor(q, 0x80, 0x80, 0x80, 0x80);
    QuadRect(q, x, y, x + 108, y + 37, 0, 1, 0x6C, 0x26);
    q++;
    q->type = 1;
    QuadColor(q, 0x80, 0x80, 0x80, 0x80);
    QuadRect(q, x + 107, y, x + 210, y + 43, 2, 0x26, 0x6A, 0x51);
    q++;

    /* Eye: three shadows and the eye, rotated by pulseAngle */
    for (i = 0; i < 4; i++) {
        q->type = 1;
        if (i == 3) {
            QuadColor(q, 0x80, 0x80, 0x80, 0x80);
        } else {
            QuadColor(q, 0, 0, 0, 0x60);
        }
        QuadRect(q, x - 6, y + 12, x + 26, y + 44, 1, 0x5D, 0x21, 0x7D);
        GsQuadRotate(q, x + (ofs[i][0] + 10), y + (ofs[i][1] + 27), 32, 32, fldThreatGauge->pulseAngle);
        q++;
    }
    q->type = 2;
    QuadColor(q, 0x80, 0x80, 0x80, 0x60);
    QuadRect(q, x - 6, y + 12, x + 26, y + 44, 0x29, 0x5D, 0x49, 0x7D);
    GsQuadRotate(q, x + 10, y + 27, 32, 32, 0.0f);
    q++;

    /* Bars: green (red on alarm) to red */
    if (fldThreatGauge->alarm != 0) {
        c0[0] = 255.0f;
        c0[1] = 32.0f;
        c0[2] = 32.0f;
        c0[3] = 128.0f;
    } else {
        c0[0] = 32.0f;
        c0[1] = 255.0f;
        c0[2] = 32.0f;
        c0[3] = 128.0f;
    }
    c1[0] = 255.0f;
    c1[1] = 32.0f;
    c1[2] = 32.0f;
    c1[3] = 255.0f;
    for (i = 0; i < 19; i++) {
        VuVecLerp(&out, &c0, &c1, (f32)i / 19.0f);
        q->type = 1;
        a = fldThreatGauge->barAlpha[i];
        QuadColor(q, (s32)out[0], (s32)out[1], 0x20, a);
        QuadRect(q, x + 28 + i * 8, y + 18, x + 38 + i * 8, y + 26, 0x4B, 0x54, 0x55, 0x5C);
        q++;
    }

    /* Label: three shadows and the label */
    for (i = 0; i < 4; i++) {
        q->type = 1;
        if (i == 3) {
            q->type = 2;
            if (fldThreatGauge->alarm != 0) {
                QuadColor(q, 0xA0, 0x20, 0x20, 0xFF);
            } else {
                QuadColor(q, 0x80, 0x80, 0x80, 0xFF);
            }
        } else {
            QuadColor(q, 0, 0, 0, 0x80);
        }
        QuadRect(q, x + (ofs[i][0] + 120), y + (ofs[i][1] + 4), x + 192 + ofs[i][0], y + 12 + ofs[i][1],
                 0, 0x53, 0x48, 0x5B);
        q++;
    }
    q->type = 0;
    GphBatchDrawQuads((GphBatchQuad *)&fldThreatGauge->pad34[0x40 - 0x34]);
}
