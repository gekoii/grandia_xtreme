#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccPartPairForwardStart(OccEntry *o) {
    OccPartPair *s = OCC_PARAMS(o, OccPartPair);
    GphModel *r;
    s32 ok;
    u16 a;
    s16 n;

    r = FldObjGet(OCC_PARAMS(o, OccPartPair)->id);
    if (r == 0) {
        OccEnd(o);
        return;
    }
    if (s->part[0] == 0) {
        ok = 0;
    } else {
        ok = ChrModelFindPart(r, s->part[0]);
    }
    if (ok == 0) {
        OccEnd(o);
        return;
    }
    o->timer = s->count;
    s->done[0] = 0;
    a = s->delay;
    n = s->se;
    s->done[1] = 0;
    s->wait[0] = a;
    s->wait[1] = a + s->lag;
    if (n > 0) {
        FldSndSePlay(1, n * 2 + 0xBE9, 0);
    }
}
