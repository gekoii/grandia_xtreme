/* rodata: 0x00674C60-0x00674C70 */
#include "common.h"
#include "protos/field.h"

typedef struct {
    u8 pad0[0xF3];
    s8 unkF3;
} Party;

typedef struct {
    u16 id;
    u16 unk2;
    u8 x;
    u8 y;
} Slot;

typedef struct {
    u8 total;
    u8 count;
    Slot slots[4];
} Grp;

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u16 id;
} Src;

typedef struct {
    f32 x, y, z;
} Vec;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 pos[3];
    Vec pts[6];
} Area;

typedef struct {
    u16 id;
    s16 val;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
} Sub;

typedef struct {
    s16 a;
    s16 b;
    s16 c;
    s16 d;
    s16 e;
    s16 f;
    s16 g;
} Sub2;

typedef struct {
    u16 unk0;
    s16 type;
    u16 unk4;
    s16 unk6;
    s16 unk8;
    u16 flags;
    f32 dir;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    Sub sub[4];
    Sub2 sub2[8];
} Enc;

typedef struct {
    u16 a;
    u16 b;
    u16 c;
} Pair;

typedef struct {
    Party *party;
    s32 unk4;
    s32 unk8;
    u8 padC[0xE4 - 0xC];
    u16 unkE4;
    u16 unkE6;
    u8 padE8[0xF0 - 0xE8];
    u16 unkF0;
    u8 padF2[0x134 - 0xF2];
    Src *srcs;
    u8 pad138[0x148 - 0x138];
    Area *areas;
    Enc *encs;
    u8 pad150[0x17C - 0x150];
    Grp *grps;
    u8 pad180[0x190 - 0x180];
    u8 n190;
    u8 pad191;
    Pair e192[1];
} Map;


void AmapBuildMonsters(Map *m) {
    f32 dirs[4] = { 32768.0f, 16384.0f, 0.0f, 49152.0f };
    Src *srcs;
    Party *party;
    Grp *grps;
    Src *src;
    Grp *g;
    Enc *encs;
    s32 i;
    s32 arg4;
    s32 k;
    s32 j;
    u16 cur;
    s32 hit;
    s32 rnd;
    s32 x;
    s32 y;
    s32 z;
    u32 dir;
    u16 id;
    s16 type;
    s16 tag;
    s32 val;

    party = m->party;
    arg4 = m->unk4;
    grps = m->grps;
    srcs = m->srcs;
    for (i = 0; i < m->unkF0; i++) {
        grps[i].slots[0].id = 0;
        grps[i].slots[1].id = 0;
        grps[i].slots[2].id = 0;
        grps[i].slots[3].id = 0;
    }
    AmapRollEncounterGroups(m->unkF0, party, arg4, grps, m->unk8);
    encs = m->encs;
    m->unkE6 = 0;
    for (i = 0; i < m->unkF0; i++) {
        rnd = 0;
        hit = 0;
        cur = m->unkE6;
        src = &srcs[i];
        dir = src->b0 >> 6;
        id = src->id;
        x = src->b1 * 500 - 250;
        y = src->b2 * 500;
        z = src->b3 * 500 - 250;
        if (src->b2 == 1) {
            hit = 1;
            if (VuRandInt(100) + 1 < party->unkF3) {
                rnd = 1;
            }
            y = 0;
        }
        m->areas[i].pos[0] = x;
        if (src->b2 == 2) {
            m->areas[i].pos[1] = 500.0f;
        } else {
            m->areas[i].pos[1] = y;
        }
        m->areas[i].pos[2] = -(f32)z;
        m->areas[i].unk4 = m->areas[i].pos[0] - 1150.0f;
        m->areas[i].unk8 = m->areas[i].pos[2] - 1150.0f;
        m->areas[i].unk10 = m->areas[i].pos[2] + 1150.0f;
        m->areas[i].unkC = m->areas[i].pos[0] + 1150.0f;
        for (j = 0; j < 6; j++) {
            m->areas[i].pts[j].x = m->areas[i].pos[0];
            m->areas[i].pts[j].y = m->areas[i].pos[1];
            m->areas[i].pts[j].z = m->areas[i].pos[2];
        }
        tag = 0;
        if (id != 0) {
            tag = i + 0x10;
            m->e192[m->n190].a = cur;
            m->e192[m->n190].b = id;
            m->n190++;
        }
        if (hit) {
            type = 1;
        } else {
            type = 2;
        }
        encs[m->unkE6].unk0 = cur;
        g = &grps[i];
        encs[m->unkE6].type = type;
        encs[m->unkE6].unk4 = g->slots[0].id;
        encs[m->unkE6].unk6 = 0;
        encs[m->unkE6].unk8 = tag;
        encs[m->unkE6].flags = 0;
        if (hit | srcs[i].b2) {
            encs[m->unkE6].flags |= 2;
            encs[m->unkE6].flags |= 0x20;
        }
        if (srcs[i].b2 == 2) {
            encs[m->unkE6].flags |= 1;
        }
        encs[m->unkE6].dir = dirs[dir];
        encs[m->unkE6].unk10 = 0;
        encs[m->unkE6].unk12 = 0xD2;
        encs[m->unkE6].unk14 = 0x708;
        encs[m->unkE6].unk16 = 0;
        if (type == 1) {
            g->count = type;
        }
        for (k = 0; k < grps[i].count; k++) {
            val = type == 1 ? rnd + 1 : 0;
            if (type == 2) {
                val = 0;
            }
            encs[m->unkE6].sub[k].id = grps[i].slots[k].id;
            encs[m->unkE6].sub[k].val = val;
            encs[m->unkE6].sub[k].unk4 = 0;
            encs[m->unkE6].sub[k].unk6 = 0;
            encs[m->unkE6].sub[k].unk8 = 0;
            encs[m->unkE6].sub[k].unkA = 0;
            encs[m->unkE6].sub[k].unkC = 0;
        }
        for (k = 0; k < 8; k++) {
            encs[m->unkE6].sub2[k].a = i;
            encs[m->unkE6].sub2[k].b = i;
            encs[m->unkE6].sub2[k].c = i;
            encs[m->unkE6].sub2[k].d = i;
            encs[m->unkE6].sub2[k].e = i;
            encs[m->unkE6].sub2[k].f = i;
            encs[m->unkE6].sub2[k].g = i;
        }
        m->unkE6++;
    }
    m->unkE4 = m->unkF0;
}
