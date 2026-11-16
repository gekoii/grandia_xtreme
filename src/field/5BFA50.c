#include "common.h"
#include "field/event.h"
#include "field/n13.h"
#include "protos/field.h"

void OccParticleForwardStart(OccEntry *arg0) {
    OccParticle *a = OCC_PARAMS(arg0, OccParticle);
    f32 *p = a->rot;
    if (a->start != 0) {
        PtclFldEffectStart(a->effect, a->pos, p);
        return;
    }
    PtclFldEffectStop(a->effect);
}
