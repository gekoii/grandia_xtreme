#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"
#include "field/n14.h"

s32 MapPathBuild(FldPath *);

void OccPathForwardStart(OccEntry *o) {
    OccPath *s = OCC_PARAMS(o, OccPath);
    s32 r;

    r = MapPathBuild(&fldPathTable[OCC_PARAMS(o, OccPath)->path]);
    if (r < 0) {
        OccEnd(o);
        return;
    }
    if (r & 0x10000) {
        r &= 0xFF;
        s->big = 1;
        s->slot = r;
        o->timer = mapPathWork->bigUsed[r];
    } else {
        r &= 0xFF;
        s->big = 0;
        s->slot = r;
        o->timer = mapPathWork->smallUsed[r];
    }
    s->frame = 0;
    s->wait = 0;
}
