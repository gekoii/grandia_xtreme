#include "common.h"
#include "chr.h"
#include "protos/main.h"

void ChrFaceAnimSet(ChrFaceAnim *obj, u8 a, u8 b) {
    if (obj != NULL && obj->faces != NULL && a < 8 && ((ChrFaceIds *)obj->faces)->ids[a] != -1) {
        obj->blinkStep = 0;
        obj->mode = b;
        obj->face = a;
        obj->blinkTimer = VuRandInt(0x1A4) + 0x78;
        obj->stepTimer = 0;
    }
}
