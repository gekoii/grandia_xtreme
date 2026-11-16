#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"

void EffSpawnByCode(GphPart *, s32, s32);

void OccMapEffectForwardStart(u8 *arg0) {
    OccMapEffect *a = OCC_PARAMS(arg0, OccMapEffect);
    GphModel *m;
    GphPart *o;

    m = FldObjGet(a->id);
    if (m != 0) {
        if (((OccEntry *)arg0)->flags & OCC_PART) {
            o = ChrModelFindPart(m, a->part);
        } else {
            o = ChrModelFindPart(m, 0);
        }
        if (o != NULL) {
            EffSpawnByCode(o, (u8)(a->effect + 1), (s32)o->mtx);
        }
    }
}
