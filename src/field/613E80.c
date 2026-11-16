#include "common.h"
#include "protos/field.h"
#include "field/n22.h"



void AmapBuildTreasures(AmapWork *o) {
    u16 buf[8];
    s32 i;
    s32 next;
    AmapTreasureRec *arr;
    AmapItemPlace *rec;
    AmapItemPlace *r;
    s32 x;
    s32 y;
    s32 a;

    i = 0;
    arr = o->treasures;
    rec = o->items;
    o->treasureCount = 0;
    if (o->itemCount != 0) {
        do {
            next = i + 1;
            r = &rec[i];
            y = r->x * 500 - 250;
            a = r->facing >> 6;
            x = r->y * 500 - 250;
            arr[o->treasureCount].id = next;
            if (i < o->unk23) {
                arr[o->treasureCount].kind = 5;
                AmapRollItems(1, 0, o->mapSet, buf);
            } else {
                arr[o->treasureCount].kind = 3;
                AmapRollItems(1, 1, o->mapSet, buf);
            }
            arr[o->treasureCount].pos[0] = y;
            arr[o->treasureCount].pos[1] = 0.0f;
            arr[o->treasureCount].pos[2] = -(f32)x;
            arr[o->treasureCount].unk10 = 500.0f;
            arr[o->treasureCount].rot = a * 90.0f;
            arr[o->treasureCount].item = buf[0];
            arr[o->treasureCount].unk1A = 0;
            arr[o->treasureCount].unk1C = 0;
            arr[o->treasureCount].unk1E = 0;
            i = next;
            o->treasureCount++;
        } while (i < o->itemCount);
    }
}
