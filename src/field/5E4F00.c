#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n17.h"

s32 EvtAreaHitBoundSphere(void *, f32, EvtArea *);

s32 MapHtaGatherMove(void *arg0, f32 arg1) {
    s32 i;
    s32 n;
    s32 cnt;
    EvtArea *e;

    i = 0;
    cnt = 0;
    n = EvtAreaGetCount();
    for (; i < n; i++) {
        e = EvtAreaGet(i);
        if (e->target == 2 || e->target == 7) {
            if ((e->flags & EVT_AREA_ENABLED) && EvtAreaHitBoundSphere(arg0, arg1, e) != 0) {
                mapHtaHitList.areas[cnt++] = e;
                if (cnt >= 64) {
                    break;
                }
            }
        }
    }
    mapHtaHitList.count = cnt;
    return cnt;
}
