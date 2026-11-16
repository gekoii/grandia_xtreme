/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "ptcl.h"
#include "field/state.h"
#include "protos/field.h"

void VuVecCopy(void *, void *);

void PtclDustSpawn(void *src, u32 mode) {
    u32 idx;
    PtclDustWork *c;
    PtclEmitterVert *m;

    if (mode == 0 && FldVecDist(src, camField[0].lookAt) > 4000.0f) {
        return;
    }
    m = ptclDustWork->emitter.verts;
    idx = ptclDustWork->next;
    VuVecCopy(m[idx].pos, src);
    c = ptclDustWork;
    c->mode = mode;
    switch (mode) {
    case 0:
        c->puffs[idx].size = 40.0f;
        c->puffs[idx].alpha = -64.0f;
        break;
    case 1:
        c->puffs[idx].size = 120.0f;
        c->puffs[idx].alpha = -64.0f;
        break;
    case 2:
        c->puffs[idx].size = 30.0f;
        c->puffs[idx].alpha = -64.0f;
        break;
    }
    ptclDustWork->puffs[idx].timer = 0;
    ptclDustWork->puffs[idx].active = 1;
    if (++ptclDustWork->next >= 31) {
        ptclDustWork->next = 0;
    }
}
