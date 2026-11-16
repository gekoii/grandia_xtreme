/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n13.h"
#include "field/n17.h"

typedef struct {
    s16 id;
    union { u16 v; } unk2;
    u16 flags;
    u16 pad6;
    f32 x, y, z;
    f32 rx, ry, rz;
    u8 pad20[0x10];
} Rec;

extern s32 mapModelPack;

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void MapModelPlace(Rec *arg0, s32 arg1) {
    s32 i;
    s32 r;
    s32 n;
    Rec *rec;
    GphModel *o;
    f32 ang;
    void *buf;

    mapModelCount = arg1;
    i = 0;
    mapModelTable = FldMemAlloc(arg1 * sizeof(MapModel));
    ang = 0.0f;
    MemSet(mapModelTable, 0, mapModelCount * sizeof(MapModel));
    for (; i < arg1; i++) {
        rec = &arg0[i];
        r = FilePacGetModel(mapModelPack, rec->id);
        if (r == 0) {
            continue;
        }
        o = ChrModelCreate(r);
        n = rec->unk2.v;
        o->pos[0] = rec->x;
        o->pos[1] = rec->y;
        o->objId = n;
        o->pos[2] = rec->z;
        o->rot[0] = rec->rx * 182.04445f;
        o->rot[1] = rec->ry * 182.04445f;
        o->rot[2] = rec->rz * 182.04445f;
        o->scale[0] = 1.0f;
        o->scale[1] = 1.0f;
        o->scale[2] = 1.0f;

        if (rec->flags & 0x200) {
            o->flags |= 8;
        }
        {
            s32 k;
            mapModelTable[i].visible = 1;
            k = fldModelTable.count;
            mapModelTable[i].model = o;
            fldModelTable.items[k] = o;
            fldModelTable.count = k + 1;
            o->flags |= 0x80;
        }
        GphModelCalc(o);
        mapModelTable[i].model->flags ^= 0x80;
        if ((u32)o->objId >= 250) {
            if (o->objId != 200) {
                buf = FldMemAlloc(ChrModelCalcVtxBufSize(mapModelTable[i].model, 1));
                ChrModelAssignVtxBufs(mapModelTable[i].model, buf, 1);
            } else {
                buf = FldMemAlloc(ChrModelCalcVtxBufSize(mapModelTable[i].model, 3));
                ChrModelAssignVtxBufs(mapModelTable[i].model, buf, 3);
            }
            mapModelTable[i].waveOn = 1;
            ang = ang + __builtin_fabs((double)VuRandInt(240));
            if (ang >= 360.0f) {
                ang -= 360.0f;
            }
            mapModelTable[i].wavePhase = ang;
            if ((EvtGetMapNo() & 0xFF00) == 0x3000) {
                mapModelTable[i].waveAmp = VuRandSigned(10.0f) + 18.0f;
                mapModelTable[i].waveSpeed = VuRandSigned(0.001f) + 0.003f;
            } else {
                mapModelTable[i].waveAmp = VuRandSigned(10.0f) + 20.0f;
                mapModelTable[i].waveSpeed = VuRandSigned(0.001f) + 0.004f;
            }
        }
    }
}

