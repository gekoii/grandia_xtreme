#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

void CamPresetChaseMove(void *, void *, void *);

void CamPresetChase(void *arg0, void *arg1, void *arg2) {
    CamTrailReset(arg2, arg1);
    camPresetState |= 0x110;
    /* s32 cast: jal + return, not a tail call (9 words off without) */
    ((s32 (*)(void *, void *, void *))CamPresetChaseMove)(arg0, arg1, arg2);
}
