#include "common.h"
#include "field/n22.h"

void AmapBuildEntrances(AmapWork *o) {
    AmapEntranceRec *e;
    f32 f;

    e = o->entrances;
    o->entranceCount = 0;
    switch (o->mapType) {
    case 0:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        f = 160.0f;
        break;
    default:
        f = -120.0f;
        break;
    }
    e[0].id = 0x101;
    e[0].pos[0] = o->startX * 500 - 250;
    e[0].pos[1] = f;
    e[0].pos[2] = -(f32)(o->startY * 500 - 150);
    e[0].rot = (o->unk17 >> 6) * 90.0f;
    e[0].rot2 = (o->unk17 >> 6) * 90.0f;
    e[0].unk18 = -2;
    e[0].unk1A = 0;
    e[0].unk1C = 0;
    e[0].unk1E = 0;
    e[1].id = 0xFFFF;
    e[1].pos[0] = o->exitX * 500 - 250;
    e[1].pos[1] = f;
    e[1].pos[2] = -(f32)(o->exitY * 500 - 150);
    e[1].unk18 = -2;
    e[1].unk1E = 0;
    e[1].unk1A = 0;
    e[1].unk1C = 0;
    o->entranceCount = 2;
    e[1].rot2 = 0.0f;
    e[1].rot = 0.0f;
}
