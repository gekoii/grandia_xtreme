#include "common.h"
#include "main/n04.h"

/* Local prototype: the tail call leaves start, p and i in a2-t0 (the definition
 * reads two arguments; a cast call would not be a tail call). */
void GphDrawListRender(u64 *, s32, s32, u32 *, u32);

void GphDrawListDrawLayer(u32 i) {
    GphDrawList *t = gphDrawListCur;
    u32 *base;
    u32 *p;
    u64 *d;
    s32 start;
    s32 n;

    if (t->count != 0) {
        base = t->layerStart;
        p = (u32 *)(i * 4 + (u32)base);
        start = *p;
        d = &t->keys[start];
        if (i < 15) {
            n = p[1] - start;
        } else {
            n = t->count - start;
        }
        if (n != 0) {
            GphDrawListRender(d, n, start, p, i);
        }
    }
}
