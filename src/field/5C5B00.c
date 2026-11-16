#include "common.h"
#include "field/n14.h"
#include "protos/field.h"

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

extern Font fldDmgDigitFont;

GphBatchQuad *FldDmgNumBuildSprites(GphBatchQuad *p, s32 x, s32 y, s32 val, s32 alpha, f32 sx, f32 sy) {
    u8 buf[8];
    s32 i;

    for (i = UtilIntToDigits(val, buf); i < 8; i++) {
        p->type = 1;
        p->v[0].r = 0x80;
        p->v[0].g = 0x80;
        p->v[0].b = 0x80;
        p->v[0].a = alpha;
        p->v[0].x = x;
        p->v[0].y = y;
        p->v[1].x = x + (s32)(fldDmgDigitFont.adv[buf[i]] * sx);
        p->v[1].y = y + (s32)(fldDmgDigitFont.tbl[buf[i]].h * sy);
        p->z = 0;
        if (p->v[0].x < 641 && p->v[0].y < 449 && p->v[0].x + p->v[1].x >= 0 && p->v[0].y + p->v[1].y >= 0) {
            p->v[0].u = fldDmgDigitFont.tbl[buf[i]].u + 1;
            p->v[0].v = fldDmgDigitFont.tbl[buf[i]].v + 1;
            p->v[1].u = fldDmgDigitFont.tbl[buf[i]].uw;
            p->v[1].v = fldDmgDigitFont.tbl[buf[i]].vh;
            p++;
            x += (s32)(fldDmgDigitFont.adv[buf[i]] * sx);
        }
    }
    p->type = 1;
    p->v[0].r = 0x80;
    p->v[0].g = 0x80;
    p->v[0].b = 0x80;
    p->v[0].a = alpha;
    p->v[0].x = x - (s32)(f32)fldDmgDigitFont.adv[buf[7]];
    p->v[0].y = y + (s32)(fldDmgDigitFont.tbl[buf[7]].h * sy);
    p->v[1].x = x - (s32)(f32)fldDmgDigitFont.adv[buf[7]] + (s32)(f32)fldDmgDigitFont.tbl[12].w;
    p->v[1].y = y + (s32)(fldDmgDigitFont.tbl[buf[7]].h * sy) + (s32)(f32)fldDmgDigitFont.tbl[12].h;
    p->z = 0;
    p->v[0].u = fldDmgDigitFont.tbl[12].u + 1;
    p->v[0].v = fldDmgDigitFont.tbl[12].v + 1;
    p->v[1].u = fldDmgDigitFont.tbl[12].uw;
    p->v[1].v = fldDmgDigitFont.tbl[12].vh;
    if (p->v[0].x < 641 && p->v[0].y < 449 && p->v[0].x + p->v[1].x >= 0 && p->v[0].y + p->v[1].y >= 0) {
        p++;
    }
    p->type = 0;
    return p;
}
