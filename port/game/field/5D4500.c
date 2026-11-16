/* Port-only C for FldTreasureSpawnDrops (0x005D4500), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "vu.h"
#include "field/state.h"
#include "field/menu.h"

/* What MapCollMove (0x005E3200) takes as the moving object: it only uses
   the position at +0x10 */
typedef struct DropMover {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ FVECTOR pos;
} DropMover;

void VuVecCopy(f32 *, f32 *);
void VuSinCosDeg(f32 *, f32 *, f32);
s32 MapCollMove(void *, DropMover *, f32 *, void *, s32, s32, f32);
void FldTreasureSpawnDrops(f32 *pos);

#define W ((FldTreasureWork *)fldTreasureWork)

/* Spawns btlHandoff's dropCount items at pos. Unless btlHandoff.unkC is
   set, each lands at the end of ten random hops from pos in the next of
   the eight drop directions, slid along the map collision. */
void FldTreasureSpawnDrops(f32 *pos) {
    ItemBagRec rec;
    FVECTOR ofs;
    FVECTOR land;
    DropMover m;
    FldTreasure *e;
    f32 s;
    f32 c;
    f32 r;
    s32 i;
    s32 j;
    s32 slot;
    s32 mdl;

    for (i = 0; i < btlHandoff.dropCount; i++) {
        __builtin_memcpy(land, pos, 16);
        if (btlHandoff.unkC == 0) {
            VuSinCosDeg(&s, &c, (f32)(W->dropDir * 45));
            VuVecCopy(m.pos, land);
            for (j = 0; j < 10; j++) {
                ofs[0] = s * (VuRandSigned(20.0f) + 20.0f);
                ofs[1] = -16.333334f;
                ofs[2] = c * (VuRandSigned(20.0f) + 20.0f);
                mdl = MapCollModelGet(0);
                MapCollMove((void *)mdl, &m, ofs, NULL, 0xFF, MapCollGetObjFlags(), 40.0f);
                m.pos[0] = m.pos[0] + ofs[0];
                m.pos[1] = m.pos[1] + ofs[1];
                m.pos[2] = m.pos[2] + ofs[2];
            }
            VuVecCopy(land, m.pos);
        }
        W->dropDir++;
        if (W->dropDir >= 8) {
            W->dropDir = 0;
        }
        slot = FldTreasureDropSlotAlloc();
        W->dropSlot = slot;
        ItemInfoFill(&rec, (u16)btlHandoff.dropIds[i], 0);
        e = &W->entries[slot];
        e->id = 0xFF;
        e->flagId = -1;
        e->item = (u16)btlHandoff.dropIds[i];
        e->t = 0.0f;
        e->unk7A = (s8)rec.unk2;
        e->instGroup = ItemGetPickupModel(rec.id);
        if (VuRandSigned(0.1f) < 0.0f) {
            r = -VuRandSigned(0.1f);
        } else {
            r = VuRandSigned(0.1f);
        }
        e->fallStep = r;
        if (e->fallStep < 0.04f) {
            e->fallStep = 0.04f;
        }
        e->state = 1;
        VuVecCopy(e->path[0], pos);
        VuVecCopy(e->path[1], pos);
        VuVecCopy(e->path[2], pos);
        VuVecCopy(e->path[3], land);
        if (e->path[3][1] < 0.0f) {
            e->path[3][1] = 0.0f;
        }
        /* two control points 100 up from the start, over the landing spot */
        e->path[1][1] = e->path[1][1] + 100.0f;
        e->path[2][1] = e->path[2][1] + 100.0f;
        e->path[2][0] = e->path[3][0];
        e->path[2][2] = e->path[3][2];
        e->path[1][0] = e->path[3][0];
        e->path[1][2] = e->path[3][2];
        e->inst = ChrInstSpawn(e->instGroup, e->path[3], 0);
        e->active = 1;
        e->age = W->count;
        W->count++;
    }
}
