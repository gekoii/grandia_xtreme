#include "common.h"
#include "field/n21.h"

#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapDeadEndMonstersPlace(AmapWork *a) {
    AmapPlace *ev2;
    AmapPlace *ev;
    s32 n;
    s32 i;
    s32 cnt;
    s32 x;
    s32 y;
    s32 v;
    s32 k;
    AmapNode *p;

    if (amapNodeCount != 0) {
        i = 0;
        cnt = 0;
        ev2 = a->monsters;
        n = a->unk3E + a->unk3F;
        ev = a->gimmicks;
        for (; i < amapNodeCount && cnt < n; i++) {
            p = (AmapNode *)(i * 6 + (u32)amapNodes); /* &amapNodes[i], index added before the base */
            x = p->x;
            y = p->y;
            if (MAP(1, x, y) != 1 && MAP(4, x, y) == 0) {
                v = p->floorMask;
                k = 0;
                if (v == 1) k = 1;
                if (v == 4) k = 1;
                if (v == 0x10) k = 1;
                if (v == 0x40) k = 1;
                if ((v & 0x77) == 0x70) k = 1;
                if ((v & 0xDD) == 0x1C) k = 1;
                if ((v & 0xDD) == 0xC1) k = 1;
                if ((v & 0x77) == 7) k = 1;
                if (k) {
                    cnt++;
                    ev[a->gimmickCount].type = (k << 6) + 0x19;
                    ev[a->gimmickCount].x = x;
                    ev[a->gimmickCount].y = y;
                    ev[a->gimmickCount].arg = (u8)a->monsterCount;
                    a->gimmickCount++;
                    MAP(4, x, y) = 1;
                    ev2[a->monsterCount].type = k << 6;
                    ev2[a->monsterCount].x = x;
                    ev2[a->monsterCount].flag = 1;
                    ev2[a->monsterCount].y = y;
                    ev2[a->monsterCount].arg = 0;
                    a->monsterCount++;
                }
            }
        }
    }
}
