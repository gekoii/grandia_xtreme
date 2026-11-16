#include "common.h"
#include "field/n22.h"

typedef struct {
    u8 pad0[0x36];
    u16 unk36;
    u16 unk38;
} Obj;

u32 AmapGetExitDest(Obj *arg0) {
    s32 i;
    u16 a = arg0->unk36;
    u16 b = arg0->unk38;

    for (i = 0; i < 30; i++) {
        if (amapDungeonTable[i].mapId == a && b == amapDungeonTable[i].lastFloor) {
            if (amapDungeonTable[i].resetFlag != 0) {
                amapFloorIndex = 0;
            }
            return amapDungeonTable[i].exitMap | 0x40000000;
        }
    }
    return a;
}
