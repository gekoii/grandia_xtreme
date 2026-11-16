#include "common.h"
#include "main/n04.h"
#include "protos/main.h"

void GphDrawListDrawLayerNormLight(u32 idx) {
    GphDrawList *m = gphDrawListCur;
    u32 *s;
    s32 start;
    u64 *e;
    s32 n = m->count;
    if (n != 0) {
        s = m->layerStart;
        start = s[idx];
        e = &m->keys[start];
        if (idx < 15) {
            n = s[idx + 1] - start;
        } else {
            n -= start;
        }
        if (n != 0) {
            GphDrawListRenderNormLight(e, n);
        }
    }
}
