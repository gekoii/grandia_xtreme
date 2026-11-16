#include "common.h"
#include "sys.h"
#include "main/n07.h"


void TxtFontImagePutGlyph16(TxtFontImage *f, s32 idx, s32 x, s32 y) {
    s32 half;
    s32 i;
    s32 size;
    u8 *data;
    u8 *src;
    u8 *dst;
    u8 *base;
    u8 *s;

    if (txtFontImageEnable == 0 || f == NULL) {
        return;
    }
    half = f->cellW / 2;
    size = half * f->cellH;
    data = f->glyphSrc;
    if (f->glyphMetricsSrc != NULL) {
        s = (u8 *)(idx * 2 + (s32)f->glyphMetricsSrc);
        f->cellMetrics[x][y][0] = s[0];
        f->cellMetrics[x][y][1] = s[1];
    }
    base = f->pixels + x * 8 + y * (18 * (f->w / 2));
    for (i = 0; i < f->cellH; i++) {
        dst = base + (f->w / 2) * i;
        src = data + size * idx + half * i;
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst[4] = src[4];
        dst[5] = src[5];
        dst[6] = src[6];
        dst[7] = src[7];
    }
}
