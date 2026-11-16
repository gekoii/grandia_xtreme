#include "common.h"
#include "gph.h"

s32 GphDrawListHasTex(u8 arg0) {
    GphDrawList *o = gphRenderCtx->drawList;
    s32 n = o->count;
    u64 *p;
    u64 x;
    if (n == 0) return 0;
    p = o->keys;
    /* goto loop with an empty do-while(0) after the first statement: the
     * alignment nop at the label the bnezl jumps back to */
loop:
    x = *p++;
    do {
    } while (0);
    if (*(u8 *)(s32)(x & 0xFFFFFFFF) == arg0) return 1;
    if (--n != 0) goto loop;
    return 0;
}
