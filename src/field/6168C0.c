#include "common.h"
#include "field/n22.h"

typedef struct {
    u8 pad0[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 pad1C[0x24 - 0x1C];
    s32 unk24;
} Unk6168C0;

void AmapGetStartPos(Unk6168C0 *arg0) {
    arg0->x = amapWork.startPosX;
    arg0->y = amapWork.startPosY;
    arg0->z = amapWork.startPosZ;
    arg0->unk24 = amapWork.unk17 << 14;
}
