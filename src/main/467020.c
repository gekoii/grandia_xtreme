#include "common.h"
#include "main/n11.h"

s32 IsPtsInRegion(ViBuf *arg0, s32 arg1) {
    s32 found;
    s32 ret;
    s32 period;
    s32 count;
    s32 i;
    s32 pos;
    MovTimeStamp *e;

    found = 0;
    ret = 0;
    period = arg0->n << 11;
    count = arg0->countTs;
    pos = arg0->wtTs;
    if (count > 0) {
        for (i = 0; i < count && !found; i++) {
            e = &arg0->ts[(pos - count + arg0->nTs + i) % arg0->nTs];
            if ((arg1 + period - e->pos) % period < e->len) {
                found = 1;
                ret = 1;
            }
        }
    }
    return ret;
}
