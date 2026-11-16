#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

extern u16 amapFloorFixedParams[];
extern u8 amapFloorRandParams[];


void AmapSetupFloorParams(AmapWork *o) {
    FldMapSet *info;
    s32 r;
    s32 i;
    s32 idx;

    info = o->mapSet;
    if (info == NULL) {
        s32 d = 30;
        o->width = 0x1C;
        o->algo1 = 1;
        o->height = 0x1C;
        o->nodeCount = 784 / d;
        return;
    }
    i = 0;
    o->monsterMax = info->monsterCount;
    o->spotCount = info->spotCount;
    o->mapNo = info->mapId;
    r = VuRandInt(100) + 1;
    idx = -1;
    for (i = 0; i < 5; i++) {
        if (info->algos[i].chance >= r) {
            idx = i;
            break;
        }
    }
    o->itemMax = info->unk1A + info->unk1C;
    if (idx == -1) {
        idx = 0;
    }
    o->unk23 = info->unk1C;
    o->dropArea1 = info->dropArea1;
    o->algoPick = info->algos[idx].algo;
    o->algo1 = info->algos[idx].algo;
    o->nodeDensity = info->algos[idx].unk2;
    o->unk3D = info->algos[idx].unk3;
    o->alcoveCount = info->algos[idx].unk4;
    o->trapArg1[0] = info->unk11C;
    o->trapArg2[0] = info->unk120;
    o->unk32 = info->unk122;
    o->trapArg1[1] = info->unk124;
    o->trapArg2[1] = info->unk128;
    o->unk33 = info->unk12A;
    o->trapArg1[2] = amapFloorFixedParams[0];
    o->trapArg2[2] = amapFloorFixedParams[1];
    r = VuRandInt(0xFF) & 3;
    o->unk34 = amapFloorRandParams[r];
    o->unk3E = info->unkF2;
    o->unk1D = o->unk3D;
    for (i = 0; i < 30; i++) {
        if (amapDungeonTable[i].mapId == o->mapNo) {
            o->gridSize = amapDungeonTable[i].gridSize;
            o->unk110 = amapDungeonTable[i].unkE;
            o->unk112 = amapDungeonTable[i].unk10;
            o->voidGrowRate = amapDungeonTable[i].unk12;
            o->unk116 = amapDungeonTable[i].unk14;
            o->voidFillMode = amapDungeonTable[i].unk16;
            break;
        }
    }
    o->width = o->gridSize;
    o->height = o->gridSize;
    if (o->spotCount != 0) {
        for (i = 0; i < 30; i++) {
            if (amapDungeonTable[i].mapId == o->mapNo) {
                s32 t = VuRandInt(100) + 1;
                if (amapDungeonTable[i].spotChance < t) {
                    o->spotCount = 0;
                }
            }
        }
    }
}
