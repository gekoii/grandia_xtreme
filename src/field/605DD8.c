/* rodata: 0x00674430-0x00674448 */
#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

extern void AmapTemplateGetDoor(AmapWork *, s32, s32, s32 *, s32 *);
extern void AmapTemplateStamp(AmapWork *, s32, s32, s32, s32, s32);

/* groups[groupCount - 1] by byte offset: the original adds the scaled index before the base */
#define BLOCK(bit, x0, y0, x1, y1)                                   \
    if ((flags >> (bit)) & 1) {                                      \
        idx = VuRandInt(2);                                      \
        cnt = tbl[idx];                                              \
        id = AmapTemplatePick(arg0, idx, -1);                           \
        AmapBoundsSet(x0, y0, x1, y1);                               \
        AmapTemplateStamp(arg0, id, -1, -1, -1, -1);                     \
        k = (arg0->groupCount - 1) * 0x2A; \
        ex = ((AmapWork *)((u8 *)arg0 + k))->groups[0].x; \
        ey = ((AmapWork *)((u8 *)arg0 + k))->groups[0].y;                                                   \
        for (i = 0; i < cnt; i++) {                                  \
            AmapTemplateGetDoor(arg0, id, i, &px, &py);                    \
            AmapNodeAdd(ex + px, ey + py);                         \
        }                                                            \
    }

void AmapGenLayoutGridRooms(AmapWork *arg0) {
    s32 tbl[6] = { 1, 2, 0, 0, 0, 0 };
    s32 px;
    s32 py;
    s32 flags;
    u32 idx;
    s32 cnt;
    s32 id;
    u8 ex;
    u8 ey;
    s32 i;
    s32 k;
    u8 w, h;
    u32 t;
    u32 c;
    s32 a, b, a2, b2, w3, h3;

    flags = AmapRandBits(2, 9);
    c = 3;
    w = arg0->width;
    a = (u8)(w / c);
    h = arg0->height;
    h3 = h - 3;
    b = (u8)(h / c);
    t = (u32)flags / 3; /* dead: only its divide-by-zero trap survives */
    w3 = w - 3;
    a2 = (u8)(w / 3) * 2;
    b2 = (u8)(h / 3) * 2;
    BLOCK(0, 3, 3, a, b)
    BLOCK(1, 3, b, a, b2)
    BLOCK(1, 3, b2, a, h3)
    BLOCK(3, a, 3, a2, b)
    BLOCK(4, a, b, a2, b2)
    BLOCK(5, a, b2, a2, h3)
    BLOCK(6, a2, 3, w3, b)
    BLOCK(6, a2, b, w3, b2)
    BLOCK(7, a2, b2, w3, h3)
    AmapBoundsSet(2, 2, arg0->width - 2, arg0->height - 2);
    AmapStartPlace(arg0);
    AmapExitPlace(arg0);
    AmapNodesScatter(arg0->nodeCount);
    AmapBoundsSet(-1, -1, -1, -1);
    AmapLayerClear(3);
    AmapNodesLinkNearest(arg0, 4);
    AmapNodesLinkGroups(arg0, 4);
    AmapNodesMark();
}
