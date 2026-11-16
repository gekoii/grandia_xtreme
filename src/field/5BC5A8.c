#include "common.h"
#include "field/event.h"
#include "field/n13.h"
#include "protos/field.h"

void OccSoundForward(OccEntry *o) {
    OccSound *s = OCC_PARAMS(o, OccSound);
    switch (s->mode) {
    case 0:
        FldSndSePlay(1, s->se, 0);
        break;
    case 1:
        FldSndSePlay(1, s->se, s);
        break;
    case 2:
        SndSePlay(s->se, 1, 0, s->pos[1], s->pos[0]);
        break;
    }
}
