#include "common.h"
#include "protos/field.h"

#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

typedef struct {
    f32 x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0xA];
    s16 unkA;
    s16 unkC;
} Shape;

typedef struct {
    u8 pad0[0x10];
    Vec pos;
} Model;

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u8 pad6[6];
    u16 unkC;
    u8 padE[0x54 - 0xE];
    s32 unk54;
    u8 pad58[8];
    Model *unk60;
    u8 pad64[0xC];
    Shape *unk70;
} Obj;

#define addvec(d, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vadd.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(d), "r"(a), "r"(b) : "memory")

void VuSinCosDeg(f32 *, f32 *, f32);

void DbgFldTenantSearchDraw(Obj *obj) {
    Vec line[2];
    Vec pts[5];
    f32 ang[5];
    f32 s;
    f32 c;
    f32 h;
    f32 r;
    u32 color;
    s32 i;
    Vec *p;

    if (obj == NULL || !(obj->unkC & 0x1000)) {
        return;
    }
    if (!(obj->unkC & 2) || obj->unk70 == NULL) {
        return;
    }
    switch (obj->unk4) {
    case 2:
        color = 0xFF00FFFF;
        break;
    case 1:
        color = 0xFF0000FF;
        break;
    case 3:
        color = 0xFFFFFFFF;
        break;
    case 0:
    default:
        color = 0xFFFF;
        break;
    }
    h = obj->unk70->unkC / 2;
    if (obj == NULL) {
        return;
    }
    ang[0] = h;
    ang[1] = h * 0.5f;
    ang[2] = 0.0f;
    ang[3] = -(h * 0.5f);
    ang[4] = -h;
    for (i = 0; i < 5; i++) {
        VuSinCosDeg(&s, &c, -((f32)obj->unk54 * 0.0054931640625f) + ang[i]);
        r = obj->unk70->unkA;
        line[1].x = r * s;
        line[1].z = r * c;
        copyq(&line[0], &obj->unk60->pos);
        addvec(&line[1], &line[0], &line[1]);
        line[0].y = obj->unk60->pos.y + 60.0f;
        line[1].y = obj->unk60->pos.y + 60.0f;
        line[0].w = 1.0f;
        line[1].w = 1.0f;
        GphLine3DDraw(2, line, color, 1);
        copyq(&pts[i], &line[1]);
    }
    p = pts;
    for (i = 0; i < 4; i++) {
        copyq(&line[0], p);
        p++;
        copyq(&line[1], p);
        GphLine3DDraw(2, line, color, 1);
    }
}
