#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

typedef struct {
    s16 w;
    s16 h;
    u16 u;
    u16 v;
    u16 uw;
    u16 vh;
} Glyph;

typedef struct {
    Glyph *tbl;
    s8 adv[1];
} Font;

extern void VuBezier3(FVECTOR *, FVECTOR *, f32);
extern Font fldDmgDigitFont;

void AmapFloorBannerDraw(void) {
    s8 buf[8];
    GphBatchQuad spr[10];
    FVECTOR out;
    FVECTOR m[4];
    GphBatchQuad *p;
    s32 n;
    s32 i;
    s32 x;
    s32 y;
    u8 alpha;
    f32 sc;

    if (amapState->bannerOn != 0) {
        if (EvtScnrIsRunning() != 1) {
            y = 0;
            p = spr;
            alpha = amapState->bannerAlpha;
            n = UtilIntToDigits(amapState->bannerFloor, buf);
            sc = 1.6f;
            if ((EvtGetMapNo() & 0xFF00) != 0xA000) {
                p->type = 1;
                p->v[0].r = 0x80;
                p->v[0].g = 0x80;
                p->v[0].b = 0x80;
                p->v[0].x = 10;
                p->v[0].y = -4;
                p->v[1].x = 0x23;
                p->v[1].y = 0xC;
                p->v[0].u = 0x76;
                p->v[0].v = 0x5A;
                p->v[1].u = 0xA6;
                p->v[1].v = 0x84;
                p->v[0].a = alpha;
                p->z = 0;
                p++;
                x = 0x20;
            } else {
                x = 0x16;
            }
            for (i = n; i < 8; i++) {
                p->v[0].r = 0x80;
                p->v[0].g = 0x80;
                p->v[0].b = 0x80;
                p->v[0].a = alpha;
                p->v[0].x = x;
                p->v[0].y = y;
                p->type = 1;
                p->v[1].x = x + (s32)(fldDmgDigitFont.adv[buf[i]] * sc);
                p->v[1].y = (s32)(fldDmgDigitFont.tbl[buf[i]].h * sc);
                p->z = 0;
                p->v[0].u = fldDmgDigitFont.tbl[buf[i]].u + 1;
                p->v[0].v = fldDmgDigitFont.tbl[buf[i]].v + 1;
                p->v[1].u = fldDmgDigitFont.tbl[buf[i]].uw;
                p->v[1].v = fldDmgDigitFont.tbl[buf[i]].vh;
                p++;
                x += (s32)(fldDmgDigitFont.adv[buf[i]] * sc);
            }
            x -= 8;
            p->v[0].x = x;
            p->v[0].r = 0x80;
            p->v[0].g = 0x80;
            p->v[0].b = 0x80;
            p->v[0].a = alpha;
            m[0][0] = 640.0f;
            m[1][0] = 320.0f;
            m[2][0] = 472.0f;
            m[3][0] = 472.0f;
            p->z = 0;
            p->type = 1;
            p->v[0].y = 0x14;
            p->v[1].x = (s32)((f32)x + sc * 12.0f);
            p->v[1].y = 0x26;
            p->v[0].u = 0xA6;
            p->v[0].v = 0x5A;
            p->v[1].u = 0xD6;
            p->v[1].v = 0x84;
            p[1].type = 0;
            VuBezier3(&out, m, amapState->bannerT);
            GphBatchSetOffset((s32)out[0], 0x28);
            GphTexBind(0xEB);
            GphBatchDrawSprites(spr);
        }
    }
}
