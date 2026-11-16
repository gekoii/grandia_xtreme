#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

void CamFollowClearChase(void *, void *, void *);

void CamFollowClear(void *arg0, void *arg1, void *arg2) {
    CamTrailReset(arg2, arg1);
    camFollowState |= 0x100;
    /* s32 casts: the calls stay jal + return, not tail calls (19 words off without) */
    if (camFollowCur->turn != 0.0f) {
        ((s32 (*)(void *, void *, void *))CamFollowClearTurn)(arg0, arg1, arg2);
    } else {
        camFollowState |= 0x10;
        ((s32 (*)(void *, void *, void *))CamFollowClearChase)(arg0, arg1, arg2);
    }
}
