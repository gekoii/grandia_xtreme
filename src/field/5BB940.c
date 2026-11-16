#include "common.h"
#include "field/event.h"
#include "field/n13.h"
#include "protos/field.h"

void OccPartPairBackwardStart(OccEntry *o) {
    OccPartPair *s = OCC_PARAMS(o, OccPartPair);
    u16 a;
    s16 n;
    o->timer = s->count;
    s->done[0] = 0;
    a = s->delay;
    n = s->se;
    s->done[1] = 0;
    s->wait[0] = a;
    s->wait[1] = a + s->lag;
    if (n > 0) {
        FldSndSePlay(1, n * 2 + 0xBEA, 0);
    }
}
