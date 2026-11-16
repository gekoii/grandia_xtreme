#include "common.h"
#include "sys.h"
#include "main/n07.h"


void TxtFontImagePutGlyph18(TxtFontImage *p, s32 arg1, s32 arg2, s32 arg3) {
    s32 half;
    s32 sz;
    u8 *src;
    u8 *pal;
    u8 *d;
    u8 *q;
    u8 *s;
    s32 i;
    s32 tw = 9;
    s32 th = 18;

    if (txtFontImageEnable != 0 && p != NULL) {
        half = p->cellW / 2;
        sz = half * p->cellH;
        pal = p->glyphMetricsSrc;
        src = p->glyphSrc;
        if (pal != NULL) {
            p->cellMetrics[arg2][arg3][0] = pal[arg1 * 2];
            p->cellMetrics[arg2][arg3][1] = pal[arg1 * 2 + 1];
        }
        d = p->pixels + arg2 * tw + arg3 * th * (p->w / 2);
        for (i = 0; i < p->cellH; i++) {
            q = d + (p->w / 2) * i;
            s = src + sz * arg1 + half * i;
            q[0] = s[0];
            q[1] = s[1];
            q[2] = s[2];
            q[3] = s[3];
            q[4] = s[4];
            q[5] = s[5];
            q[6] = s[6];
            q[7] = s[7];
        }
    }
}
