#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "battle/n28.h"

typedef int s128 __attribute__((mode(TI)));

typedef union {
    s128 q;
    struct {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    } v;
} Vec;

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }
void FxHitSlotDraw(FxHitSlot *);

void FxHitSpawn(u8 type, s32 idx, Vec *pos, s32 arg3) {
    FxHitSlot *e;
    Vec tmp;
    BtlUnit *u;

    e = &fxHitSlots[idx];
    if ((type & 0xF0) == 0x40 && (e->type & 0xF0) != 0x40) {
        GphScreenFadeSetColor(0.0f, 128.0f, 255.0f, 96.0f);
        GphScreenFadeStart(0, 0.0f, 128.0f, 255.0f, 0.0f, 16.0f);
    }
    e->type = type;
    e->state = 0;
    EnvLightFree(e->light);
    e->light = -1;
    copyq(&e->pos[0], pos);
    e->model = 0;
    e->frameStamp = btlFrameCount;
    if (e->modelBuf == 0) {
        e->frameStamp = btlFrameCount + 1;
    }
    e->update = FxHitSlotUpdate;
    e->draw = FxHitSlotDraw;
    switch (type & 0xF0) {
    case 0x40:
        e->angleY = arg3;
        e->type = 0x49;
        return;
    case 0x30:
        u = &btlUnits[idx];
        BtlChrGetPartPos(&tmp, &btlUnits[u->attacker], 1);
        tmp.v.x -= pos->v.x;
        tmp.v.y -= pos->v.y;
        tmp.v.z -= pos->v.z;
        e->angleX = BtlAtan2Angle(-fsqrt(tmp.v.x * tmp.v.x + tmp.v.z * tmp.v.z), tmp.v.y);
        e->angleY = BtlAtan2Angle(tmp.v.x, tmp.v.z);
        break;
    }
    e->pos[0] += ((rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 60.0f;
    e->pos[1] += ((rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 60.0f;
    e->pos[2] += ((rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 60.0f;
}
