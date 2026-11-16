#include "common.h"
#include "protos/main.h"
#include "chr.h"

typedef struct {
    u8 v[3];
} B3;

extern B3 chrBlinkStepFrames;
extern B3 chrBlinkStepFaces;

void ChrFaceAnimUpdate(ChrFaceAnim *arg0, s32 arg1) {
    B3 a;
    B3 b;
    GphPart *obj;
    s32 v;
    u8 idx;
    s32 i;
    GphGaObject *p;

    a = chrBlinkStepFrames;
    b = chrBlinkStepFaces;
    if (arg0 == NULL) {
        return;
    }
    if (arg0->faces == NULL) {
        return;
    }
    obj = ChrModelFindPart(arg1, 0x28A);
    if (obj == NULL) {
        return;
    }
    if (arg0->mode != 0) {
        if (arg0->blinkTimer < arg0->blinkStep) {
            idx = arg0->face;
            arg0->stepTimer++;
            v = ((ChrFaceIds *)arg0->faces)->ids[b.v[arg0->face]];
            if (a.v[idx] < arg0->stepTimer) {
                arg0->stepTimer = 0;
                arg0->face = idx + 1;
            }
            if (arg0->face >= 3) {
                if ((arg0->blinkTimer & 7) == 1) {
                    arg0->blinkTimer = arg0->blinkTimer & 8;
                } else {
                    arg0->blinkStep = 0;
                    arg0->stepTimer = 0;
                    arg0->blinkTimer = VuRandInt(300) + 120;
                }
                arg0->face = 0;
                v = ((ChrFaceIds *)arg0->faces)->ids[b.v[0]];
            }
        } else {
            arg0->blinkStep++;
            v = ((ChrFaceIds *)arg0->faces)->ids[b.v[0]];
        }
    } else {
        v = ((ChrFaceIds *)arg0->faces)->ids[arg0->face];
    }
    if (v != -1) {
        p = obj->gaObjs;
        for (i = 0; i < obj->gaObjCount; i++) {
            p->unk3 = v;
            p++;
        }
    }
}
