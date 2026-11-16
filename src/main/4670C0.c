#include "common.h"
#include "protos/main.h"
#include "main/n11.h"


s32 viBufModifyPts(ViBuf *arg0, MovTimeStamp *arg1) {
    s32 cont;
    s32 idx;
    s32 period;
    s32 n;
    s32 end;
    MovTimeStamp *e;

    cont = 1;
    idx = (arg0->wtTs - arg0->countTs + arg0->nTs) % arg0->nTs;
    period = arg0->n << 11;
    if (arg0->countTs > 0) {
        while (cont && (e = &arg0->ts[idx])->len != 0 && arg1->len != 0) {
            if (IsPtsInRegion(arg0, arg1->pos) != 0) {
                if (e->pts >= 0) {
                    e->pts = -1;
                    e->dts = -1;
                    e->pos = 0;
                    e->len = 0;
                }
                n = arg0->countTs - 1;
                if (n < 0) {
                    n = 0;
                }
                arg0->countTs = n;
            } else {
                if ((e->pos + period - arg1->pos) % period < arg1->len) {
                    end = arg1->pos + arg1->len;
                    e->len -= end - e->pos;
                    e->pos = end;
                    if (e->len == 0) {
                        if (e->pts >= 0) {
                            e->pts = -1;
                            e->dts = -1;
                            e->pos = 0;
                            e->len = 0;
                        }
                        n = arg0->countTs - 1;
                        if (n < 0) {
                            n = 0;
                        }
                        arg0->countTs = n;
                    }
                } else {
                    cont = 0;
                }
                idx = (idx + 1) % arg0->nTs;
            }
        }
    }
    return 0;
}
