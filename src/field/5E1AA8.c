#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/n17.h"

/* Wander area (chrTenantAreaTable), as read here. */
typedef struct {
    u8 pad0[2];
    u8 type;
    u8 pad3[0x14 - 3];
    f32 pos[3];
} Area;

typedef struct {
    u8 pad0[0x10];
    u16 w;
    u16 h;
} Img;

typedef struct {
    u8 pad0[2];
    u16 pack;
} EnemyDef;

/* Enemy work (ChrEnemyWorkAlloc), held in ChrObj.sense for encounter symbols. */
typedef struct {
    EnemyDef *def;
    Img *img;
} EnemyWork;

void ChrEnemyWorkSetup(ChrObj *, ChrTenantDef *, s32);

void ChrTenantSetup(ChrObj *o, ChrTenantDef *def, s32 arg2) {
    s32 i;
    s32 ii;
    s32 kk;
    s32 j;
    s32 v;
    s32 copy;
    s32 k2;
    u8 *mp;
    struct ChrTenantTrack *(*va)[7];
    s32 three;
    f32 f;
    s32 k;
    GphModel *m;
    GphPart *n;
    f32 *pos;
    ChrTenantDefSlot *pd;
    struct ChrTenantArea **qq;

    ChrObjClear(o);
    o->unk0 = 0x15;
    o->chrId = def->id;
    v = def->angle * 182.044449f;
    o->id = def->objId;
    o->heading = v;
    o->moveDir = v;
    o->unk19 = def->unk8;
    o->leaderDist = 800000.0f;
    o->speed = 0;
    o->motSpeed = 1.0f;
    o->vel[3] = 1.0f;
    o->vel[0] = 0;
    o->vel[1] = 0;
    o->vel[2] = 0;
    if (def->mode == 2) {
        def->mode = 1;
    }
    three = 3;
    copy = 0;
    if (def->mode == three) {
        def->mode = 1;
        copy = 1;
    }
    ChrTenantSetFlagsFromDef(o, def);
    ChrTenantSetActive(o, EvtMapFlagGet(arg2) ^ 1);
    o->collMask = MapCollGetObjFlags();
    if (copy) {
        pd = def->slots;
        va = o->tracks;
        mp = &o->faceAnim;
        o->areas[0] = ChrTenantAreaPickRandom();
        qq = &o->areas[1];
        for (i = 6; i >= 0; i--) {
            *qq++ = o->areas[0];
        }
        if (((Area *)o->areas[0])->type == 11) {
            o->flags |= 0x404;
        } else {
            o->flags |= 0x404;
        }
    } else {
        pd = def->slots;
        va = o->tracks;
        mp = &o->faceAnim;
        for (i = 0; i < 8; i++) {
            o->areas[i] = ChrTenantAreaGet(pd[i].area);
        }
    }
    for (i = 0; i < 8; ) {
        ii = i;
        for (j = 0, k2 = i + 1; j < 6; ) {
            i = j + 1;
            va[ii][j] = ChrTenantTrackGet(pd[ii].tracks[j]);
            j = i;
        }
        do { i = k2; } while (0); /* do-while(0): doubles i = k2's weight for register allocation */
    }
    if (o->flags & 0x10) {
        o->sense = ChrEnemyWorkAlloc();
        ChrEnemyWorkSetup(o, def, arg2);
        k = ((EnemyWork *)o->sense)->def->pack;
    } else {
        k = def->pack;
        o->actFlags |= 0x20000;
        o->actFlags |= 0x10000;
    }
    if (def->global) {
        k--;
        m = ChrModelCreateFromPack(fldPartyPackTable[k].modelPac, 2, 0);
        ChrFaceAnimInit(mp, &fldPartyPackTable[k], m);
        ChrFaceAnimSet(mp, 0, 1);
    } else {
        if (fldChrPackTable[k].bind.modelPac == 0) {
            k = 0;
        }
        m = ChrModelCreateFromPack(fldChrPackTable[k].bind.modelPac, 2, 0);
        ChrFaceAnimInit(mp, &fldChrPackTable[k], m);
        ChrFaceAnimSet(mp, 0, 1);
    }
    if (!(o->flags & 0x10)) {
        ChrModelShowExtraParts(m, 0);
    }
    o->model = m;
    m->rot[1] = o->heading;
    m->objId = (*(s32 *)def & 0xFFFF) + 1000;
    m->rot[1] = o->heading;
    n = ChrModelFindPart(m, 0x280);
    if (n != NULL) {
        n->flags = 0;
    }
    if (def->global) {
        for (kk = 0; kk < 32; kk++) {
            if (fldPartyPackTable[k].pack->motions[kk] != 0) {
                ChrMotTableSet(o, fldPartyPackTable[k].pack->motions[kk], kk);
            }
        }
    } else {
        for (kk = 0; kk < 32; kk++) {
            if (fldChrPackTable[k].pack->motions[kk] != 0) {
                ChrMotTableSet(o, fldChrPackTable[k].pack->motions[kk], kk);
            }
        }
    }
    if (o->actFlags & 2) {
        ChrMotSetRandomFrame(o, 3);
    } else {
        ChrMotSetRandomFrame(o, 0);
    }
    pos = ((Area *)o->areas[0])->pos;
    m->pos[0] = pos[0];
    o->opTimer = 0;
    m->pos[1] = pos[1];
    m->pos[2] = pos[2];
    o->area = 0;
    o->opState = 0;
    o->opPc = 0;
    if (o->flags & 0x10) {
        f = ((EnemyWork *)o->sense)->img->w;
        o->radius = f;
        f = ((EnemyWork *)o->sense)->img->h;
        o->unk38 = f;
    } else {
        o->radius = 40.0f;
    }
}
