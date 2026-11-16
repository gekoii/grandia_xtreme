/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"

typedef struct {
    f32 x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct {
    s32 count;
    f32 r;
    s32 flag;
    s16 y1;
    s16 y0;
    f32 cx, cy, cz;
    u8 pad1C[4];
    Vec pts[1];
} Shape;

void VuSinCosDeg(f32 *, f32 *, f32);
void GphPoly3DDraw(s32, Vec *, u32, s32);
void VuVecCopy(Vec *, Vec *);

#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

s32 DbgFldBorderDraw(s32 mode) {
    Vec pts[2];
    Vec cp[2];
    f32 s;
    f32 c;
    s32 n;
    s32 i;
    s32 j;
    u32 color;
    Shape *obj;
    Vec *p;
    Vec *q;
    f32 lim;

    pts[0].w = 1.0f;
    pts[1].w = 1.0f;
    color = 0x3040A030;
    MapWallSelectLayer(mode);
    n = MapWallGetCount();
    if (mode == 0) {
        color = 0x30A03040;
    }
    for (i = 0; i < n; i++) {
        obj = MapWallGet(i);
        if (obj->count != 0) {
            p = &obj->pts[0];
            for (j = 1; j < obj->count; j++) {
                q = p;
                p = &obj->pts[j];
                VuVecCopy(&pts[0], q);
                lim = 6000.0f;
                VuVecCopy(&pts[1], p);
                if (FldVecDist(&pts[0], camField[0].lookAt) > lim && FldVecDist(&pts[1], camField[0].lookAt) > lim) {
                    continue;
                }
                if (obj->flag == 0) {
                    pts[1].y = pts[0].y = camField[0].lookAt[1] - 200.0f;
                } else {
                    pts[1].y = pts[0].y = obj->y0;
                }
                copyq(&cp[0], &pts[0]);
                copyq(&cp[1], &pts[1]);
                if (obj->flag == 0) {
                    cp[0].y += 300.0f;
                    cp[1].y += 300.0f;
                } else {
                    cp[1].y = cp[0].y = obj->y1;
                }
                if (obj->flag) {
                    GphPoly3DDraw(4, pts, 0x304030A0, 1);
                } else {
                    GphPoly3DDraw(4, pts, color, 1);
                }
            }
        } else {
            for (j = 0; j < 16; j++) {
                VuSinCosDeg(&s, &c, j * 22);
                pts[0].x = obj->cx + obj->r * c;
                pts[0].y = obj->cy + camField[0].lookAt[1];
                pts[0].z = obj->cz + obj->r * s;
                pts[0].w = 1.0f;
                VuSinCosDeg(&s, &c, (j + 1) * 22);
                pts[1].x = obj->cx + obj->r * c;
                pts[1].y = obj->cy + camField[0].lookAt[1];
                pts[1].z = obj->cz + obj->r * s;
                pts[1].w = 1.0f;
                if (obj->flag) {
                    pts[0].y = obj->y0;
                    pts[1].y = obj->y1;
                }
                GphLine3DDraw(2, pts, 0xFF00FFFF, 1);
            }
        }
    }
    return 0;
}
