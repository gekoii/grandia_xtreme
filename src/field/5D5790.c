#include "common.h"
#include "protos/field.h"
#include "field/n16.h"

typedef struct {
    s16 unk0;
    s16 type;
    f32 x, y, z;
    f32 unk10;
    f32 unk14;
    u16 model;
    u8 pad1A[4];
    s16 flag;
} Rec;


void FldTreasurePlace(Rec *recs, s32 n) {
    VuVec4 vec;
    u16 out[8];
    s8 listA[32];
    s8 listB[32];
    s8 shuf[32];
    u32 nA = 0;
    u32 nB = 0;
    MapSetFile *info;
    s32 i;
    u32 j;
    Rec *r;
    s8 *p;

    info = MapSetFileGet();
    if (info == NULL) {
        return;
    }
    for (i = 0; i < n; i++) {
        r = &recs[i];
        if (r->flag == 0 && EvtMapTreasureFlagGet(i) == 1) {
            continue;
        }
        if (r->type < 2) {
            continue;
        }
        if (r->type < 4) {
            listA[nA++] = i;
        } else if (r->type < 6) {
            listB[nB++] = i;
        }
    }
    j = 0;
    if (info->itNum1 < nA) {
        for (i = 0; i < 32; i++) {
            shuf[i] = -1;
        }
        do {
        /* goto loop: written as an inner do-while it is 183 words off */
        retry:
            p = &shuf[VuRandInt(nA)];
            if (*p != -1) {
                goto retry;
            }
            *p = listA[j];
            j++;
        } while (nA != j);
        for (i = info->itNum1; i < nA; i++) {
            EvtMapTreasureFlagSet(shuf[i], 1);
        }
    }
    j = 0;
    if (info->itNum2 < nB) {
        for (i = 0; i < 32; i++) {
            shuf[i] = -1;
        }
        do {
            p = &shuf[VuRandInt(nB)];
            if (*p == -1) {
                *p = listB[j];
                j++;
                if (nB == j) {
                    break;
                }
            }
        } while (nB != j);
        for (i = info->itNum2; i < nB; i++) {
            EvtMapTreasureFlagSet(shuf[i], 1);
        }
    }
    for (i = 0; i < n; i++) {
        r = &recs[i];
        vec.x = r->x;
        vec.y = r->y;
        vec.z = r->z;
        vec.w = 0.0f;
        if (r->flag == 0) {
            r->flag = i;
            if (EvtMapTreasureFlagGet(i) == 1) {
                continue;
            }
        } else {
            r->flag += 0x400;
        }
        switch (r->type) {
        case 0:
            FldTreasureSpawn(0, &vec, r->model, i, r->unk14, r->flag);
            break;
        case 1:
            FldTreasureSpawn(1, &vec, r->model, i, r->unk14, r->flag);
            break;
        case 2:
            AmapRollItems(1, 1, info, out);
            FldTreasureSpawn(0, &vec, out[0], i, r->unk14, r->flag);
            break;
        case 3:
            AmapRollItems(1, 1, info, out);
            FldTreasureSpawn(1, &vec, out[0], i, r->unk14, r->flag);
            break;
        case 4:
            AmapRollItems(1, 0, info, out);
            FldTreasureSpawn(0, &vec, out[0], i, r->unk14, r->flag);
            break;
        case 5:
            AmapRollItems(1, 0, info, out);
            FldTreasureSpawn(1, &vec, out[0], i, r->unk14, r->flag);
            break;
        case 6:
            break;
        case 8:
            MapNearObjAdd(0x5C, &vec, i, r->unk10);
            break;
        case 9:
            MapNearObjAdd(0x5B, &vec, i, r->unk10);
            break;
        case 7:
        case 10:
            MapNearObjAdd(0x5D, &vec, i, r->unk10);
            break;
        case 11:
            MapNearObjAdd(0x5A, &vec, i, r->unk10);
            break;
        }
    }
}
