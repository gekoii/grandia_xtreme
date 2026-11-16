/* cc: ee-gcc2.9-991111-01 */
#include "common.h"

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 pad30[9];
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
} Camera;

void GphViewLoadScratch(Camera *c) {
    f32 *p;

    p = (f32 *)0x70000030;
    *(Camera *volatile *)0x700000EC = c;
    p[0] = c->unk8 / c->unk0;
    p[1] = c->unkC / c->unk4;
    p[2] = 1.0f / (c->unk2C - c->unk28);
    p[3] = c->unk18;
    p[6] = (c->unk2C + c->unk28) * -0.5f / (c->unk2C - c->unk28);
    p = (f32 *)0x70000060;
    p[0] = c->unk54;
    p[1] = c->unk58;
    p[2] = c->unk5C * 16.0f;
    p[3] = 0.0f;
    p[4] = c->unk68;
    p[5] = c->unk6C;
    p[11] = c->unk18;
    p[20] = c->unk24;
    p[21] = c->unk64;
    p[22] = 255.0f;
    p[23] = c->unk60;
    __asm__ volatile("sync");
}
