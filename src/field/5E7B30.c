#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "gph.h"
#include "field/n13.h"
#include "field/n17.h"

/* Model pack header. */
typedef struct {
    u32 count;
} Hdr;

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void MapModelLoadPac(Hdr *arg0) {
    u32 i;
    GphModel *o;
    s32 n;

    if (arg0 == 0) {
        return;
    }
    i = 0;
    mapModelTable = FldMemAlloc(arg0->count * sizeof(MapModel));
    MemSet(mapModelTable, 0, arg0->count * sizeof(MapModel));
    mapModelCount = arg0->count;
    for (; i < arg0->count; i++) {
        mapModelTable[i].model = ChrModelCreate(FilePacGetModel(arg0, i));
        EffSpawnModelEffects(mapModelTable[i].model);
        mapModelTable[i].visible = 1;
        n = fldModelTable.count;
        fldModelTable.count = n + 1;
        fldModelTable.items[n] = mapModelTable[i].model;
    }
    if (sysIni.ground0Dsp) {
        mapModelTable[0].visible = 0;
    }
    o = mapModelTable[0].model;
    o->flags |= 0x80;
    GphModelCalc(o);
    MapCollModelSet(MapModelGetObj(0), 0);
    MapCollModelSet(MapModelGetObj(0), 1);
}
