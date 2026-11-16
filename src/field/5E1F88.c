#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/n17.h"


void ChrTenantSetupFromPack(ChrObj *o, s32 idx, s32 slot) {
    s32 i;
    GphModel *m;

    ChrObjClear(o);
    o->motSpeed = 1.0f;
    o->chrId = 0;
    o->heading = 0;
    o->moveDir = 0;
    o->speed = 0;
    o->vel[0] = 0;
    o->vel[1] = 0;
    o->vel[2] = 0;
    o->unk0 = 0x15;
    o->id = -1;
    o->leaderDist = 800000.0f;
    o->vel[3] = 1.0f;
    o->flags = 0x1CAF;
    o->collMask = MapCollGetObjFlags();
    m = ChrModelCreateFromPack(mapExtraCharPacks[idx].bind.modelPac, 2, 0);
    ChrFaceAnimInit(&o->faceAnim, &mapExtraCharPacks[idx], o->model);
    ChrFaceAnimSet(&o->faceAnim, 0, 1);
    m->rot[1] = o->heading;
    m->objId = 0;
    o->model = m;
    m->rot[1] = o->heading;
    for (i = 0; i < 32; i++) {
        if (mapExtraCharPacks[idx].pack->motions[i] != 0) {
            ChrMotTableSet(o, mapExtraCharPacks[idx].pack->motions[i], i);
        }
    }
    ChrMotSetRandomFrame(o, 0);
    m->pos[3] = 1.0f;
    m->pos[1] = 0;
    m->pos[2] = 0;
    m->pos[0] = 0;
    o->radius = 40.0f;
    o->opState = 0;
    o->opPc = 0;
    o->opTimer = 0;
    o->area = 0;
}
