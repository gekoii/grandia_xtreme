/* cflags: -fno-strength-reduce */
/* rodata: 0x00674EA0-0x00674EC8 */
/* -fno-strength-reduce: with strength reduction the quad loops get pointer
 * givs and more saved registers (323 words differ) */
#include "common.h"
#include "grxsys.h"
#include "protos/field.h"
#include "field/n22.h"

typedef struct {
    u16 id;
    u8 unk2;
    u8 pad3;
    s16 pos;
} Info;


static const char save[] = "save";


/* indexed by hand: &evtFaceWins[i] swaps an addu's operands */
#define W(i) ((EvtFaceWin *)((i) * sizeof(EvtFaceWin) + (u32)evtFaceWins))

void EvtFaceWinUpdate(void) {
    s32 idx;
    Info *p;
    s32 x;
    s32 y;
    EvtFaceWin *e;
    GphBatchQuad *q;
    s32 i;
    s32 k;
    s16 x0;
    s16 y0;
    u16 *ids;
    s32 cw;
    s8 *px;
    s8 *py;

    x = 0;
    y = 0;
    idx = 0;
    do {
        p = EvtScnrGetPicture(idx);
        if (p->unk2 == 1) {
            switch (p->pos >> 4) {
            case 0:
                x = 0x40;
                y = 0xD6;
                break;
            case 1:
                x = 0x1E0;
                y = 0xD6;
                break;
            case 2:
                x = 0x40;
                y = 0x60;
                break;
            case 3:
                x = 0x1E0;
                y = 0x40;
                break;
            case 4:
                x = 0x110;
                y = 0xBB;
                break;
            }
            EvtFaceWinOpen(idx, p->id, x, y, 0x60, 0x60, 0);
            e = W(idx);
            if ((e->unk14 = e->nameLen) != 0) {
                s8 offs[8] = { 2, 2, -2, -2, 2, -2, -2, 2 };
                s8 ma;
                s8 mb;

                q = e->quads;
                y0 = e->nameY;
                x0 = e->nameX;
                ids = ChrPortraitGetName(e->id);
                for (i = 0; i < W(idx)->nameLen; i++) {
                    TxtGlyphGetMetrics(ids[i], (u8 *)&ma, (u8 *)&mb, sysGrxsysPac->glyphTable);
                    cw = (f32)(ma - mb) * 1.5f;
                    for (k = 0, px = &offs[0], py = &offs[1]; k < 4; k++) {
                        q->type = 1;
                        q->v[0].r = 0x10;
                        q->v[0].g = 0x40;
                        q->v[0].b = 0x20;
                        q->v[0].a = 0x60;
                        q->v[1].r = 0x10;
                        q->v[1].g = 0x40;
                        q->v[1].b = 0x20;
                        q->v[1].a = 0x60;
                        q->v[2].r = 0x10;
                        q->v[2].g = 0x40;
                        q->v[2].b = 0x20;
                        q->v[2].a = 0x60;
                        q->v[3].r = 0x10;
                        q->v[3].g = 0x40;
                        q->v[3].b = 0x20;
                        q->v[3].a = 0x60;
                        q->v[0].x = x0 + *px;
                        q->v[0].y = y0 + *py;
                        q->v[1].x = x0 + *px;
                        q->v[1].y = y0 + *py + 0x18;
                        q->v[2].x = x0 + *px + cw;
                        q->v[2].y = y0 + *py;
                        q->v[3].x = x0 + *px + cw;
                        q->v[3].y = y0 + *py + 0x18;
                        q->z = 0;
                        q->v[0].u = mb + i * 0x12;
                        q->v[0].v = idx * 0x12 + 0xB4;
                        q->v[1].u = mb + i * 0x12;
                        q->v[1].v = idx * 0x12 + 0xC4;
                        q->v[2].u = mb + i * 0x12 + (ma - mb);
                        q->v[2].v = idx * 0x12 + 0xB4;
                        q->v[3].u = mb + i * 0x12 + (ma - mb);
                        q->v[3].v = idx * 0x12 + 0xC4;
                        q++;
                        px += 2;
                        py += 2;
                    }
                    q->type = 1;
                    q->v[0].r = 0x80;
                    q->v[0].g = 0x80;
                    q->v[0].b = 0x80;
                    q->v[0].a = 0x80;
                    q->v[1].r = 0x80;
                    q->v[1].g = 0x80;
                    q->v[1].b = 0x80;
                    q->v[1].a = 0x80;
                    q->v[2].r = 0x80;
                    q->v[2].g = 0x80;
                    q->v[2].b = 0x80;
                    q->v[2].a = 0x80;
                    q->v[3].r = 0x80;
                    q->v[3].g = 0x80;
                    q->v[3].b = 0x80;
                    q->v[3].a = 0x80;
                    q->v[0].x = x0;
                    q->v[0].y = y0;
                    q->v[1].x = x0;
                    q->v[1].y = y0 + 0x18;
                    q->v[2].x = x0 + cw;
                    q->v[2].y = y0;
                    q->v[3].x = x0 + cw;
                    q->v[3].y = y0 + 0x18;
                    q->z = 0;
                    q->v[0].u = mb + i * 0x12;
                    q->v[0].v = idx * 0x12 + 0xB4;
                    q->v[1].u = mb + i * 0x12;
                    q->v[1].v = idx * 0x12 + 0xC4;
                    q->v[2].u = mb + i * 0x12 + (ma - mb);
                    q->v[2].v = idx * 0x12 + 0xB4;
                    q->v[3].u = mb + i * 0x12 + (ma - mb);
                    q->v[3].v = idx * 0x12 + 0xC4;
                    q++;
                    x0 += cw + 2;
                }
                q->type = 0;
            }
        } else {
            EvtFaceWinClose(idx);
        }
        idx++;
    } while (idx < 3);
}
