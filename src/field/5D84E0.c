#include "common.h"
#include "protos/field.h"
#include "pad.h"

typedef struct {
    f32 x, y, z, w;
} Vec __attribute__((aligned(16)));


typedef struct {
    u8 pad0[0x10];
    Vec pos;
} Model;

typedef struct {
    u8 pad0[0x50];
    s32 unk50;
    u8 pad54[0xC];
    Model *unk60;
    u8 pad64[0x180 - 0x64];
    Vec unk180;
} Obj;

#define addvec(d, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vadd.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(d), "r"(a), "r"(b) : "memory")

extern s32 dbgFldChrMoveLine;
extern char dbgChrStrVecY[];
extern char dbgChrStrVecZ[];
extern char dbgChrStrPosX[];
extern char dbgChrStrMoveDebug[];
extern char dbgChrStrMoveDirect[];


void DbgChrMovePage(Obj *obj) {
    Pad *pad = &padPort1;
    f32 y;
    s32 ang;
    s32 n;

    if (obj == NULL) {
        return;
    }
    DbgPrintf(4, 5, dbgChrStrMoveDebug);
    y = obj->unk60->pos.y;
    ChrPlayerPadMove(obj, pad);
    addvec(&obj->unk60->pos, &obj->unk60->pos, &obj->unk180);
    obj->unk60->pos.y = y;
    ang = (f32)(obj->unk50 + 0x8000) * 0.0054931640625f;
    while (ang > 180) {
        ang -= 360;
    }
    while (ang < -180) {
        ang += 360;
    }
    n = 3;
    DbgDrawCursor(n, dbgFldChrMoveLine + 6);
    dbgFldChrMoveLine = DbgMenuLineSelect(dbgFldChrMoveLine, 4);
    DbgPrintf(4, 6, dbgChrStrPosX, obj->unk60->pos.x);
    DbgPrintf(4, 7, dbgChrStrVecY, obj->unk60->pos.y);
    DbgPrintf(4, 8, dbgChrStrVecZ, obj->unk60->pos.z);
    DbgPrintf(4, 9, dbgChrStrMoveDirect, ang);
    if (pad->held & 2) {
        return;
    }
    if (!(pad->held & 1)) {
        if (pad->repeatFast & 0x8000) {
            switch (dbgFldChrMoveLine) {
            case 0:
                obj->unk60->pos.x -= 10.0f;
                break;
            case 1:
                obj->unk60->pos.y -= 10.0f;
                break;
            case 2:
                obj->unk60->pos.z -= 10.0f;
                break;
            case 3:
                obj->unk50 -= 0x400;
                break;
            }
        }
        if (pad->repeatFast & 0x2000) {
            switch (dbgFldChrMoveLine) {
            case 0:
                obj->unk60->pos.x += 10.0f;
                break;
            case 1:
                obj->unk60->pos.y += 10.0f;
                break;
            case 2:
                obj->unk60->pos.z += 10.0f;
                break;
            case 3:
                obj->unk50 += 0x400;
                break;
            }
        }
    } else {
        if (pad->repeatFast & 0x8000) {
            switch (dbgFldChrMoveLine) {
            case 0:
                obj->unk60->pos.x -= 1.0f;
                break;
            case 1:
                obj->unk60->pos.y -= 1.0f;
                break;
            case 2:
                obj->unk60->pos.z -= 1.0f;
                break;
            case 3:
                obj->unk50 -= 0x100;
                break;
            }
        }
        if (pad->repeatFast & 0x2000) {
            switch (dbgFldChrMoveLine) {
            case 0:
                obj->unk60->pos.x += 1.0f;
                break;
            case 1:
                obj->unk60->pos.y += 1.0f;
                break;
            case 2:
                obj->unk60->pos.z += 1.0f;
                break;
            case 3:
                obj->unk50 += 0x100;
                break;
            }
        }
    }
}
