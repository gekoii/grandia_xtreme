/* Port-only C for AmapGimmickBoxGet (0x0060DE90), which the decompilation still has in assembly. */
#include "common.h"

/* Collision box of a gimmick model, as in src/field/data/669028.c */
typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ s16 kind;
    /* 0x04 */ s16 corner0[3];
    /* 0x0A */ s16 corner1[3];
} AmapGimmickBox; /* size 0x10 */

/* The gimmick placement being filled (amapGimmickPos, src/field/60F8A8.c) */
typedef struct {
    /* 0x00 */ s16 kind;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 rot;      /* 0-3, a quarter turn each */
    /* 0x08 */ s16 box[6];   /* two (x, y, z) corners */
} AmapGimmickPos;

extern AmapGimmickBox amapGimmickBoxTable[];
extern const f32 amapGimmickRotDegTable[4];

void VuSinCosDeg(f32 *, f32 *, f32);

void AmapGimmickBoxGet(s32 id, AmapGimmickPos *o) {
    AmapGimmickBox *b;
    f32 ang[4];
    s32 px[2];
    s32 pz[2];
    f32 sin;
    f32 cos;
    s32 x;
    s32 z;
    s32 i;
    f32 fx;
    f32 fz;

    x = o->x;
    z = o->z;
    ang[0] = amapGimmickRotDegTable[0];
    ang[1] = amapGimmickRotDegTable[1];
    ang[2] = amapGimmickRotDegTable[2];
    ang[3] = amapGimmickRotDegTable[3];

    /* No end test: the id is always in the table */
    for (i = 0; amapGimmickBoxTable[i].id != id; i++) {
    }
    b = &amapGimmickBoxTable[i];

    o->kind = b->kind;
    if (b->kind == 0) {
        /* Rotated about the cell centre (250) */
        o->box[0] = b->corner0[0];
        o->box[1] = b->corner0[1];
        px[0] = b->corner0[0] - 250;
        o->box[2] = b->corner0[2];
        pz[0] = b->corner0[2] - 250;
        o->box[3] = b->corner1[0];
        px[1] = b->corner1[0] - 250;
        o->box[4] = b->corner1[1];
        o->box[5] = b->corner1[2];
        pz[1] = b->corner1[2] - 250;
        VuSinCosDeg(&sin, &cos, ang[o->rot]);
        for (i = 0; i < 2; i++) {
            fx = px[i];
            fz = pz[i];
            px[i] = x + (s32)(fx * cos - fz * sin) + 250;
            pz[i] = z + (s32)(fx * sin + fz * cos) + 250;
        }
        o->box[0] = px[0];
        o->box[5] = pz[1];
        o->box[2] = pz[0];
        o->box[3] = px[1];
    } else {
        o->box[0] = b->corner0[0] + x;
        o->box[1] = b->corner0[1];
        o->box[2] = b->corner0[2] + z;
        o->box[3] = b->corner1[1];
        o->box[5] = 0;
        o->box[4] = b->corner1[0];
    }
}
