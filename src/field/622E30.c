#include "common.h"
#include "protos/field.h"
#include "field/menu.h"


void GsTexUpload(s32);

FldMenuQuad *FldMenuWinBodyBuild(FldMenuQuad *q, FldMenuWin *w, s32 x, s32 y, u8 a) {
    u8 a;
    u8 r0, g0, b0, r1, g1, b1;

    a = w->alpha * 102.4f;
    if (w->selected == 1) {
        r0 = fldMenuCursor->colorKeys[4][0];
        g0 = fldMenuCursor->colorKeys[4][1];
        b0 = fldMenuCursor->colorKeys[4][2];
        r1 = fldMenuCursor->colorKeys[5][0];
        g1 = fldMenuCursor->colorKeys[5][1];
        b1 = fldMenuCursor->colorKeys[5][2];
    } else {
        r0 = fldMenuCursor->colorKeys[0][0];
        g0 = fldMenuCursor->colorKeys[0][1];
        b0 = fldMenuCursor->colorKeys[0][2];
        r1 = fldMenuCursor->colorKeys[1][0];
        g1 = fldMenuCursor->colorKeys[1][1];
        b1 = fldMenuCursor->colorKeys[1][2];
    }
    if (w->styleFlags & 2) {
        a = 100;
    }
    q->type = 5;
    if (w->selected == 1) {
        q->v[0].r = 0;
        q->v[0].g = 0x4D;
        q->v[0].b = 0xAF;
        q->v[0].a = a;
        q->v[2].r = 0;
        q->v[2].g = 0x4D;
        q->v[2].b = 0xAF;
        q->v[2].a = a;
        q->v[1].r = 0x1C;
        q->v[1].g = 0x32;
        q->v[1].b = 0x76;
        q->v[1].a = a;
        q->v[3].r = 0x1C;
        q->v[3].g = 0x32;
        q->v[3].b = 0x76;
        q->v[3].a = a;
    } else if (w->unk44 != 0) {
        q->v[0].r = w->col[0][0];
        q->v[0].g = w->col[0][1];
        q->v[0].b = w->col[0][2];
        q->v[0].a = a;
        q->v[2].r = w->col[1][0];
        q->v[2].g = w->col[1][1];
        q->v[2].b = w->col[1][2];
        q->v[2].a = a;
        q->v[1].r = w->col[2][0];
        q->v[1].g = w->col[2][1];
        q->v[1].b = w->col[2][2];
        q->v[1].a = a;
        q->v[3].r = w->col[3][0];
        q->v[3].g = w->col[3][1];
        q->v[3].b = w->col[3][2];
        q->v[3].a = a;
    } else {
        q->v[0].r = 0;
        q->v[0].g = 0xFD;
        q->v[0].b = 0x8F;
        q->v[0].a = a;
        q->v[2].r = 0;
        q->v[2].g = 0xFD;
        q->v[2].b = 0x8F;
        q->v[2].a = a;
        q->v[1].r = 0x1C;
        q->v[1].g = 0x72;
        q->v[1].b = 0x56;
        q->v[1].a = a;
        q->v[3].r = 0x1C;
        q->v[3].g = 0x72;
        q->v[3].b = 0x56;
        q->v[3].a = a;
    }
    q->v[0].x = 0;
    q->v[0].y = 0;
    q->v[1].x = 0;
    q->v[1].y = w->h2;
    q->v[2].x = w->w2;
    q->v[2].y = 0;
    q->v[3].x = w->w2;
    q->v[3].y = w->h2;
    q->z = 0;
    q[1].type = 0;
    GphBatchSetOffset(x - (w->w2 >> 1), y - (w->h2 >> 1));
    GphBatchDrawQuads(&w->quads[32]); /* body quad at +0x8090 */
    if (w->unk16 != 0) {
        GphBatchSetOffset(x - (w->w2 >> 1), y - (w->h2 >> 1));
        ChrPortraitSetByCode(w->portraitCode, w->unk16);
        GsTexUpload(ChrPortraitGetTex((u16)w->portraitCode));
        GsTexPacketSend(ChrPortraitGetTex((u16)w->portraitCode));
        GphBatchDrawSprites(&w->quads[80]); /* portrait sprite at +0x8C90 */
    }

    q = &w->quads[32];
    a = w->alpha * 160.0f;

    q->type = 5;
    q->v[0].r = r0; q->v[0].g = g0; q->v[0].b = b0; q->v[0].a = a;
    q->v[2].r = r0; q->v[2].g = g0; q->v[2].b = b0; q->v[2].a = a;
    q->v[1].r = r1; q->v[1].g = g1; q->v[1].b = b1; q->v[1].a = a;
    q->v[3].r = r1; q->v[3].g = g1; q->v[3].b = b1; q->v[3].a = a;
    q->v[0].x = -4;
    q->v[0].y = 0;
    q->v[1].x = -4;
    q->v[1].y = w->h2;
    q->v[2].x = 0;
    q->v[2].y = 0;
    q->v[3].x = 0;
    q->v[3].y = w->h2;
    q->z = 0;
    q++;

    q->type = 5;
    q->v[0].r = r0; q->v[0].g = g0; q->v[0].b = b0; q->v[0].a = a;
    q->v[2].r = r0; q->v[2].g = g0; q->v[2].b = b0; q->v[2].a = a;
    q->v[1].r = r1; q->v[1].g = g1; q->v[1].b = b1; q->v[1].a = a;
    q->v[3].r = r1; q->v[3].g = g1; q->v[3].b = b1; q->v[3].a = a;
    q->z = 0;
    q->v[0].x = w->w2;
    q->v[0].y = 0;
    q->v[1].x = w->w2;
    q->v[1].y = w->h2;
    q->v[2].x = w->w2 + 4;
    q->v[2].y = 0;
    q->v[3].x = w->w2 + 4;
    q->v[3].y = w->h2;
    q++;

    q->type = 5;
    q->v[0].r = r1; q->v[0].g = g1; q->v[0].b = b1; q->v[0].a = a;
    q->v[1].r = r1; q->v[1].g = g1; q->v[1].b = b1; q->v[1].a = a;
    q->v[2].r = r1; q->v[2].g = g1; q->v[2].b = b1; q->v[2].a = a;
    q->v[3].r = r1; q->v[3].g = g1; q->v[3].b = b1; q->v[3].a = a;
    q->v[0].x = -4;
    q->v[0].y = w->h2;
    q->v[1].x = -4;
    q->v[1].y = w->h2 + 4;
    q->v[2].x = w->w2 + 4;
    q->v[2].y = w->h2;
    q->v[3].x = w->w2 + 4;
    q->v[3].y = w->h2 + 4;
    q->z = 0;
    q++;

    q->type = 5;
    q->v[0].r = r0; q->v[0].g = g0; q->v[0].b = b0; q->v[0].a = a;
    q->v[1].r = r0; q->v[1].g = g0; q->v[1].b = b0; q->v[1].a = a;
    q->v[2].r = r0; q->v[2].g = g0; q->v[2].b = b0; q->v[2].a = a;
    q->v[3].r = r0; q->v[3].g = g0; q->v[3].b = b0; q->v[3].a = a;
    q->v[0].x = -4;
    q->v[0].y = -4;
    q->v[1].x = -4;
    q->v[1].y = 0;
    q->v[2].x = w->w2 + 4;
    q->v[2].y = -4;
    q->v[3].x = w->w2 + 4;
    q->v[3].y = 0;
    q->z = 0;
    q++;

    if (w->flags & 1) {
        u16 d = -4;
        q->type = 5;
        q->v[0].r = r0; q->v[0].g = g0; q->v[0].b = b0; q->v[0].a = a;
        q->v[1].r = r0; q->v[1].g = g0; q->v[1].b = b0; q->v[1].a = a;
        q->v[2].r = r0; q->v[2].g = g0; q->v[2].b = b0; q->v[2].a = a;
        q->v[3].r = r0; q->v[3].g = g0; q->v[3].b = b0; q->v[3].a = a;
        q->v[0].x = -4;
        q->v[0].y = -8;
        q->v[1].x = -4;
        q->v[1].y = -4;
        q->v[2].x = w->w2 / 3 + d;
        q->v[2].y = -8;
        q->v[3].x = w->w2 / 3 + d + 4;
        q->v[3].y = -4;
        q->z = 0;
        q++;
    }
    if (w->flags & 0x100) {
        u16 d = -4;
        q->type = 5;
        q->v[0].r = r0; q->v[0].g = g0; q->v[0].b = b0; q->v[0].a = a;
        q->v[1].r = r0; q->v[1].g = g0; q->v[1].b = b0; q->v[1].a = a;
        q->v[2].r = r0; q->v[2].g = g0; q->v[2].b = b0; q->v[2].a = a;
        q->v[3].r = r0; q->v[3].g = g0; q->v[3].b = b0; q->v[3].a = a;
        q->v[0].x = -4;
        q->v[0].y = -12;
        q->v[1].x = -4;
        q->v[1].y = -4;
        q->v[2].x = w->w2 / 3 + d;
        q->v[2].y = -12;
        q->v[3].x = w->w2 / 3 + d + 8;
        q->v[3].y = -4;
        q->z = 0;
        q++;
    }
    if (w->flags & 0x40000) {
        u16 d = -4;
        q->type = 5;
        q->v[0].r = r0; q->v[0].g = g0; q->v[0].b = b0; q->v[0].a = a;
        q->v[1].r = r0; q->v[1].g = g0; q->v[1].b = b0; q->v[1].a = a;
        q->v[2].r = r0; q->v[2].g = g0; q->v[2].b = b0; q->v[2].a = a;
        q->v[3].r = r0; q->v[3].g = g0; q->v[3].b = b0; q->v[3].a = a;
        q->v[0].x = -4;
        q->v[0].y = -24;
        q->v[1].x = -4;
        q->v[1].y = -4;
        q->v[2].x = w->w2 / 3 + d;
        q->v[2].y = -24;
        q->v[3].x = w->w2 / 3 + d + 8;
        q->v[3].y = -4;
        q->z = 0;
        q++;
    }
    if (w->flags & 2) {
        q->type = 5;
        q->v[0].r = r0; q->v[0].g = g0; q->v[0].b = b0; q->v[0].a = a;
        q->v[1].r = r0; q->v[1].g = g0; q->v[1].b = b0; q->v[1].a = a;
        q->v[2].r = r0; q->v[2].g = g0; q->v[2].b = b0; q->v[2].a = a;
        q->v[3].r = r0; q->v[3].g = g0; q->v[3].b = b0; q->v[3].a = a;
        q->v[0].x = w->w2 + 4;
        q->v[0].y = -4;
        q->v[1].x = w->w2 + 4;
        q->v[1].y = 12;
        q->v[2].x = w->w2 + 12;
        q->v[2].y = -4;
        q->v[3].x = w->w2 + 12;
        q->v[3].y = 4;
        q->z = 0;
        q++;
    }
    if (w->flags & 8) {
        q->type = 5;
        q->v[0].r = r1; q->v[0].g = g1; q->v[0].b = b1; q->v[0].a = a;
        q->v[1].r = r1; q->v[1].g = g1; q->v[1].b = b1; q->v[1].a = a;
        q->v[2].r = r1; q->v[2].g = g1; q->v[2].b = b1; q->v[2].a = a;
        q->v[3].r = r1; q->v[3].g = g1; q->v[3].b = b1; q->v[3].a = a;
        q->v[0].x = w->w2 - 70;
        q->v[0].y = w->h2 + 4;
        q->v[1].x = w->w2 - 70;
        q->v[1].y = w->h2 + 8;
        q->v[2].x = w->w2 + (u16)-6;
        q->v[2].y = w->h2 + 4;
        q->v[3].x = w->w2 + (u16)-6;
        q->v[3].y = w->h2 + 8;
        q->z = 0;
        q->v[1].x += 4;
        q->v[3].x -= 4;
        q++;
    }
    if (w->flags & 0x800) {
        q->type = 5;
        q->v[0].r = r1; q->v[0].g = g1; q->v[0].b = b1; q->v[0].a = a;
        q->v[1].r = r1; q->v[1].g = g1; q->v[1].b = b1; q->v[1].a = a;
        q->v[2].r = r1; q->v[2].g = g1; q->v[2].b = b1; q->v[2].a = a;
        q->v[3].r = r1; q->v[3].g = g1; q->v[3].b = b1; q->v[3].a = a;
        q->v[0].x = w->w2 - 70;
        q->v[0].y = w->h2 + 4;
        q->v[1].x = w->w2 - 70;
        q->v[1].y = w->h2 + 8;
        q->v[2].x = w->w2 + (u16)-6;
        q->v[2].y = w->h2 + 4;
        q->v[3].x = w->w2 + (u16)-6;
        q->v[3].y = w->h2 + 8;
        q->z = 0;
        q->v[1].x += 4;
        q->v[3].x -= 4;
        q++;
    }
    if (w->flags & 0x80000) {
        q->type = 5;
        q->v[0].r = r1; q->v[0].g = g1; q->v[0].b = b1; q->v[0].a = a;
        q->v[1].r = r1; q->v[1].g = g1; q->v[1].b = b1; q->v[1].a = a;
        q->v[2].r = r1; q->v[2].g = g1; q->v[2].b = b1; q->v[2].a = a;
        q->v[3].r = r1; q->v[3].g = g1; q->v[3].b = b1; q->v[3].a = a;
        q->v[0].x = w->w2 - 180;
        q->v[0].y = w->h2 + 4;
        q->v[1].x = w->w2 - 180;
        q->v[1].y = w->h2 + 24;
        q->v[2].x = w->w2;
        q->v[2].y = w->h2 + 4;
        q->v[3].x = w->w2;
        q->v[3].y = w->h2 + 24;
        q->z = 0;
        q->v[1].x += 8;
        q->v[3].x -= 12;
        q++;
    }
    if (w->flags & 0x20) {
        q->type = 5;
        q->v[0].r = r1; q->v[0].g = g1; q->v[0].b = b1; q->v[0].a = a;
        q->v[1].r = r1; q->v[1].g = g1; q->v[1].b = b1; q->v[1].a = a;
        q->v[2].r = r1; q->v[2].g = g1; q->v[2].b = b1; q->v[2].a = a;
        q->v[3].r = r1; q->v[3].g = g1; q->v[3].b = b1; q->v[3].a = a;
        q->v[0].x = w->w2 - 32;
        q->v[0].y = 0;
        q->v[1].x = w->w2 - 32;
        q->v[1].y = 2;
        q->v[2].x = w->w2;
        q->v[2].y = 0;
        q->v[3].x = w->w2 - 4;
        q->v[3].y = 2;
        q->z = 0;
        q++;
        q->type = 5;
        q->v[0].r = r1; q->v[0].g = g1; q->v[0].b = b1; q->v[0].a = a;
        q->v[1].r = r1; q->v[1].g = g1; q->v[1].b = b1; q->v[1].a = a;
        q->v[2].r = r1; q->v[2].g = g1; q->v[2].b = b1; q->v[2].a = a;
        q->v[3].r = r1; q->v[3].g = g1; q->v[3].b = b1; q->v[3].a = a;
        q->v[0].x = w->w2 - 4;
        q->v[0].y = 0;
        q->v[1].x = w->w2 - 4;
        q->v[1].y = 8;
        q->v[2].x = w->w2;
        q->v[2].y = 0;
        q->v[3].x = w->w2;
        q->v[3].y = 8;
        q->z = 0;
        q->v[1].y -= 4;
        q++;
    }
    if (w->flags & 0x10) {
        q->type = 5;
        q->v[0].r = r1; q->v[0].g = g1; q->v[0].b = b1; q->v[0].a = a;
        q->v[1].r = r1; q->v[1].g = g1; q->v[1].b = b1; q->v[1].a = a;
        q->v[2].r = r1; q->v[2].g = g1; q->v[2].b = b1; q->v[2].a = a;
        q->v[3].r = r1; q->v[3].g = g1; q->v[3].b = b1; q->v[3].a = a;
        q->v[0].x = 0;
        q->v[0].y = 0;
        q->v[1].y = 2;
        q->v[2].x = 32;
        q->v[2].y = 0;
        q->v[3].x = 32;
        q->v[3].y = 2;
        q->v[1].x = q->v[0].x - 4;
        q->z = 0;
        q++;
        q->type = 5;
        q->v[0].r = r1; q->v[0].g = g1; q->v[0].b = b1; q->v[0].a = a;
        q->v[1].r = r1; q->v[1].g = g1; q->v[1].b = b1; q->v[1].a = a;
        q->v[2].r = r1; q->v[2].g = g1; q->v[2].b = b1; q->v[2].a = a;
        q->v[3].r = r1; q->v[3].g = g1; q->v[3].b = b1; q->v[3].a = a;
        q->v[0].x = 0;
        q->v[0].y = 0;
        q->v[1].x = 0;
        q->v[1].y = 8;
        q->v[2].x = 4;
        q->v[2].y = 0;
        q->v[3].x = 4;
        q->v[3].y = q->v[1].y - 4;
        q->z = 0;
        q++;
    }
    if (w->flags & 0x80) {
        q->type = 5;
        q->v[0].r = r1; q->v[0].g = g1; q->v[0].b = b1; q->v[0].a = a;
        q->v[1].r = r1; q->v[1].g = g1; q->v[1].b = b1; q->v[1].a = a;
        q->v[2].r = r1; q->v[2].g = g1; q->v[2].b = b1; q->v[2].a = a;
        q->v[3].r = r1; q->v[3].g = g1; q->v[3].b = b1; q->v[3].a = a;
        q->v[0].x = w->w2 - 32;
        q->v[0].y = w->h2 - 2;
        q->v[1].x = w->w2 - 32;
        q->v[1].y = w->h2;
        q->v[2].x = w->w2;
        q->v[2].y = w->h2 - 2;
        q->v[3].x = w->w2;
        q->v[3].y = w->h2;
        q->z = 0;
        q->v[0].x += 4;
        q++;
        q->type = 5;
        q->v[0].r = r1; q->v[0].g = g1; q->v[0].b = b1; q->v[0].a = a;
        q->v[1].r = r1; q->v[1].g = g1; q->v[1].b = b1; q->v[1].a = a;
        q->v[2].r = r1; q->v[2].g = g1; q->v[2].b = b1; q->v[2].a = a;
        q->v[3].r = r1; q->v[3].g = g1; q->v[3].b = b1; q->v[3].a = a;
        q->v[0].x = w->w2 - 4;
        q->v[0].y = w->h2 - 8;
        q->v[1].x = w->w2 - 4;
        q->v[1].y = w->h2;
        q->v[2].x = w->w2;
        q->v[2].y = w->h2 - 8;
        q->v[3].x = w->w2;
        q->v[3].y = w->h2;
        q->z = 0;
        q->v[0].y += 4;
        q++;
    }
    if (w->flags & 0x40) {
        q->type = 5;
        q->v[0].r = r1; q->v[0].g = g1; q->v[0].b = b1; q->v[0].a = a;
        q->v[1].r = r1; q->v[1].g = g1; q->v[1].b = b1; q->v[1].a = a;
        q->v[2].r = r1; q->v[2].g = g1; q->v[2].b = b1; q->v[2].a = a;
        q->v[3].r = r1; q->v[3].g = g1; q->v[3].b = b1; q->v[3].a = a;
        q->v[0].x = 0;
        q->v[0].y = w->h2 - 2;
        q->v[1].x = 0;
        q->v[1].y = w->h2;
        q->v[2].x = 32;
        q->v[2].y = w->h2 - 2;
        q->v[3].x = 32;
        q->v[3].y = w->h2;
        q->z = 0;
        q->v[2].x -= 4;
        q++;
        q->type = 5;
        q->v[0].r = r1; q->v[0].g = g1; q->v[0].b = b1; q->v[0].a = a;
        q->v[1].r = r1; q->v[1].g = g1; q->v[1].b = b1; q->v[1].a = a;
        q->v[2].r = r1; q->v[2].g = g1; q->v[2].b = b1; q->v[2].a = a;
        q->v[3].r = r1; q->v[3].g = g1; q->v[3].b = b1; q->v[3].a = a;
        q->v[0].x = 0;
        q->v[0].y = w->h2 - 8;
        q->v[1].x = 0;
        q->v[1].y = w->h2;
        q->v[2].x = 4;
        q->v[2].y = w->h2 - 8;
        q->v[3].x = 4;
        q->v[3].y = w->h2;
        q->z = 0;
        q->v[2].y += 4;
        q++;
    }
    if (w->flags & 0x20000) {
        s16 x = -32;
        q->type = 5;
        q->v[0].r = r0; q->v[0].g = g0; q->v[0].b = b0; q->v[0].a = a;
        q->v[1].r = r0; q->v[1].g = g0; q->v[1].b = b0; q->v[1].a = a;
        q->v[2].r = r0; q->v[2].g = g0; q->v[2].b = b0; q->v[2].a = a;
        q->v[3].r = r0; q->v[3].g = g0; q->v[3].b = b0; q->v[3].a = a;
        q->v[0].y = -16;
        q->v[2].y = -16;
        q->v[0].x = -24;
        q->v[1].x = x;
        q->v[1].y = 0;
        q->v[2].x = -8;
        q->v[3].x = 0;
        q->v[3].y = 0;
        q->z = 0;
        q++;
        q->type = 5;
        q->v[0].r = r0; q->v[0].g = g0; q->v[0].b = b0; q->v[0].a = a;
        q->v[1].r = r0; q->v[1].g = g0; q->v[1].b = b0; q->v[1].a = a;
        q->v[2].r = r0; q->v[2].g = g0; q->v[2].b = b0; q->v[2].a = a;
        q->v[3].r = r0; q->v[3].g = g0; q->v[3].b = b0; q->v[3].a = a;
        q->v[0].x = x;
        q->v[0].y = 0;
        q->v[2].x = -4;
        q->v[1].y = 24;
        q->v[3].y = 24;
        q->v[2].y = 0;
        q->v[1].x = -80;
        q->v[3].x = -52;
        q->z = 0;
        q++;
        q->type = 5;
        q->v[0].r = r0; q->v[0].g = g0; q->v[0].b = b0; q->v[0].a = a;
        q->v[1].r = r0; q->v[1].g = g0; q->v[1].b = b0; q->v[1].a = a;
        q->v[2].r = r0; q->v[2].g = g0; q->v[2].b = b0; q->v[2].a = a;
        q->v[3].r = r0; q->v[3].g = g0; q->v[3].b = b0; q->v[3].a = a;
        q->v[0].y = 16;
        q->v[1].x = -96;
        q->v[1].y = 22;
        q->v[2].x = -48;
        q->v[2].y = 16;
        q->v[3].x = -48;
        q->v[3].y = 22;
        q->v[0].x = -88;
        q->z = 0;
        q++;
    }
    return q;
}
