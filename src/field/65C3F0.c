#include "common.h"
#include "ptcl.h"

/* the tail of a PtclDustPuff (alpha, active, timer): the loop steps a second pointer through it, and timer compares unsigned */
typedef struct {
    f32 y;
    s32 active;
    u32 timer;
} Sub;

void PtclDustUpdate(void) {
    PtclDustWork *c = ptclDustWork;
    Sub *s;
    PtclDustPuff *p;
    s32 i;

    s = (Sub *)&c->puffs[0].alpha;
    p = c->puffs;
    for (i = 0; i < 31;) {
        i++;
        if (s->active != 0) {
            ++s->timer;
            if (c->mode == 1) {
                p->size += 8.0f;
                if (s->timer >= 32) {
                    s->y -= 8.0f;
                } else {
                    s->y += 2.0f;
                }
            } else {
                p->size += 1.0f;
                if (s->timer >= 32) {
                    s->y -= 8.0f;
                } else {
                    s->y += 2.0f;
                }
            }
        }
        s = (Sub *)((u8 *)s + sizeof(PtclDustPuff));
        p++;
    }
}
