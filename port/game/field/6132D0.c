/* Port-only C for AmapBuildCollision (0x006132D0), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "vu.h"
#include "field/n21.h"

/* As in src/field/data/6673C8.c, which defines the tables */
typedef struct AmapCollTemplate {
    /* 0x00 */ u16 count;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 pts[6][2];      /* (x, z) */
} AmapCollTemplate; /* size 0x1C */

/* A polygon group record (+0x150 and +0x158 lists): AmapPathRange with its
   +4 word, which this function writes as an s32 */
typedef struct AmapCollGroup {
    /* 0x0 */ u16 start;
    /* 0x2 */ u16 end;
    /* 0x4 */ s32 attr;
} AmapCollGroup; /* size 0x8 */

extern AmapCollTemplate amapCollTemplates[8][30];
extern u8 amapCollTemplateIndex[8][2][32];
extern u8 amapCollShapeAttr[32];
extern const f32 amapRotDegTable[4];

void VuSinCosDeg(f32 *s, f32 *c, f32 ang);

/*
 * Builds the floor's collision polygons, cell by cell: each cell's shape
 * picks two templates (amapCollTemplateIndex[set][0/1]), whose points are
 * rotated by the cell's rotation and moved by cell * 500. The first set goes
 * to +0x150/+0x154 (at most 0x400 groups; the check stops the scan), the
 * second to +0x158/+0x15C. The set is masked to 8 bits (andi). The asm
 * leaves 0 in $v0; the caller (src/field/602CC0.c) declares it void.
 */
void AmapBuildCollision(AmapWork *m, s32 arg) {
    u8 set = arg;
    f32 ang[4];
    f32 s;
    f32 c;
    AmapCollGroup *groupsA;
    VuVec3 *ptsA;
    AmapCollGroup *groupsB;
    VuVec3 *ptsB;
    s32 nA;
    s32 cntA;
    s32 nB;
    s32 cntB;
    s32 row;
    s32 col;
    s32 k;
    u8 b;
    s32 shape;
    s32 rot;
    s32 ta;
    s32 tb;
    s32 x;
    s32 z;
    s32 px;
    s32 pz;

    groupsA = (AmapCollGroup *)m->pathRanges;
    ptsA = m->pathPoints;
    groupsB = m->unk158;
    ptsB = m->unk15C;
    nA = 0;
    cntA = 0;
    nB = 0;
    cntB = 0;
    ang[0] = amapRotDegTable[0];
    ang[1] = amapRotDegTable[1];
    ang[2] = amapRotDegTable[2];
    ang[3] = amapRotDegTable[3];

    for (row = 0; row < m->height; row++) {
        for (col = 0; col < m->width && nA < 0x400; col++) {
            b = m->tiles[row * m->width + col];
            shape = b & 0x1F;
            rot = b >> 6;
            ta = amapCollTemplateIndex[set][0][shape];
            tb = amapCollTemplateIndex[set][1][shape];

            if (amapCollTemplates[m->mapType][ta].count != 0) {
                groupsA[nA].start = cntA;
                for (k = 0; k < amapCollTemplates[m->mapType][ta].count;) {
                    k++;
                    x = amapCollTemplates[m->mapType][ta].pts[k - 1][0] - 250;
                    z = amapCollTemplates[m->mapType][ta].pts[k - 1][1] - 250;
                    VuSinCosDeg(&s, &c, ang[rot]);
                    pz = (s32)(x * s + z * c);
                    px = (s32)(x * c - z * s);
                    ptsA[cntA].x = (f32)(px + col * 500);
                    ptsA[cntA].z = -(f32)(pz + row * 500);
                    cntA++;
                }
                groupsA[nA].end = cntA - 1;
                groupsA[nA].attr = amapCollShapeAttr[shape];
                nA++;
            }

            if (amapCollTemplates[m->mapType][tb].count != 0) {
                groupsB[nB].start = cntB;
                for (k = 0; k < amapCollTemplates[m->mapType][tb].count;) {
                    k++;
                    x = amapCollTemplates[m->mapType][tb].pts[k - 1][0] - 250;
                    z = amapCollTemplates[m->mapType][tb].pts[k - 1][1] - 250;
                    VuSinCosDeg(&s, &c, ang[rot]);
                    pz = (s32)(x * s + z * c);
                    px = (s32)(x * c - z * s);
                    ptsB[cntB].x = (f32)(px + col * 500);
                    ptsB[cntB].z = -(f32)(pz + row * 500);
                    cntB++;
                }
                groupsB[nB].end = cntB - 1;
                groupsB[nB].attr = 0;
                nB++;
            }
        }
    }
    m->pathCount = nA;
    ((u16 *)m->padFC)[1] = 0;   /* +0xFE */
    m->unkF6 = cntA;
    m->unkF8 = nB;
    ((u16 *)m->padFC)[0] = 0;   /* +0xFC */
    m->unkFA = cntB;
}
