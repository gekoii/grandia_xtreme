/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"


void PtclDustDraw(void) {
    PtclDustWork *c = ptclDustWork;
    s32 i;
    f32 f;
    s32 a;
    FVECTOR *m;
    u8 *col;

    c->drawn = 0;
    col = &c->emitter.colors[0][0][3];
    m = &c->emitter.verts[0].pos; /* walked as FVECTORs (m[1] = size): a PtclEmitterVert * doesn't match */
    for (i = 0; i < 31; i++) {
        if (ptclDustWork->puffs[i].active != 0) {
            if (ptclDustWork->puffs[i].alpha < -127.0f) {
                m[1][0] = m[1][1] = 0.0f;
                ptclDustWork->puffs[i].timer = 0;
                ptclDustWork->puffs[i].alpha = -127.0f;
                ptclDustWork->puffs[i].active = 0;
            } else {
                f = ptclDustWork->puffs[i].size;
                m[1][0] = f;
                m[1][1] = f;
                a = ptclDustWork->puffs[i].alpha;
                col[0] = a;
                col[4] = a;
                col[8] = a;
                col[12] = a;
                ptclDustWork->drawn = 1;
            }
        }
        col += 16;
        m += 4;
    }
    if (ptclDustWork->drawn != 0) {
        if (ptclDustWork->mode == 2) {
            EffDrawBindTex1Model6();
        } else {
            EffDrawBindTex0Model5();
        }
        PtclEmitterDraw(&ptclDustWork->emitter, camField);
    }
}
