/* rodata: 0x00675020-0x00675030 */
#include "common.h"
#include "grxsys.h"
#include "protos/field.h"

typedef struct {
    s32 active;
    u8 pad4[8];
    s32 xC;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    u16 u0;
    u16 v0;
    s16 x0;
    s16 y0;
    u8 pad1C[4];
    u16 u1;
    u16 v1;
    s16 x1;
    s16 y1;
    u8 pad28[0x18];
} Sprite;


static const char save_str[] = "save";


#define SPR(q, col, X, Y, W, H, U, V, UW, VH) \
    {                                         \
        (q)->active = 1;                      \
        (q)->r = (col)[0];                    \
        (q)->g = (col)[1];                    \
        (q)->b = (col)[2];                    \
        (q)->a = (col)[3];                    \
        (q)->x0 = X;                          \
        (q)->y0 = Y;                          \
        (q)->x1 = X + W;                      \
        (q)->y1 = Y + H;                      \
        (q)->xC = 0;                          \
        (q)->u0 = U;                          \
        (q)->v0 = V;                          \
        (q)->u1 = U + UW;                     \
        (q)->v1 = V + VH;                     \
    }

void FldMenuWinGlyphsBuild(Sprite *q, s32 *str, s32 x, s32 y, s32 size, s32 h, s32 vbase, s32 ubase, s32 n, u8 *col2, u8 *col1, u8 mode) {
    s8 offs[8] = { 2, 2, -2, -2, 2, -2, -2, 2 };
    f32 scale;
    s32 i;
    s32 c;
    s32 r;
    s32 k;
    s32 cw;
    s8 ma;
    s8 mb;
    s8 *px;
    s8 *py;

    scale = size * 0.0625f;
    for (i = 0; i < n; i++) {
        if (i < 28) {
            c = i;
            r = 0;
        } else {
            c = i - 28;
            r = 1;
        }
        TxtGlyphGetMetrics(str[i], (u8 *)&ma, (u8 *)&mb, sysGrxsysPac->glyphTable);
        cw = (ma - mb) * scale;
        if (mode == 1) {
            px = &offs[0];
            py = &offs[1];
            for (k = 0; k < 4; k++) {
                SPR(q, col1, x + px[k * 2], y + py[k * 2], cw, h, ubase + c * 18 + mb, (vbase + r) * 18, (ma - mb), 0x10);
                q++;
            }
        } else {
            SPR(q, col1, x + 1, y + 1, cw, h, ubase + c * 18 + mb, (vbase + r) * 18, (ma - mb), 0x10);
            q++;
        }
        SPR(q, col2, x, y, cw, h, ubase + c * 18 + mb, (vbase + r) * 18, (ma - mb), 0x10);
        q++;
        x += cw + 2;
    }
}
