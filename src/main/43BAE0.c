#include "common.h"
#include "protos/main.h"
#include "gph.h"

void MemMove128(void *, void *, s32);
void GphGaObjEnvMapCalcUv(GphGaObject *, FMATRIX *);

void GphModelEnvMapCalcUv(GphModel *obj) {
    s32 i;
    s32 j;
    GphPart *n;
    GphGaObject *s;
    FMATRIX *w;

    if (obj->cull < 0) {
        return;
    }
    w = obj->viewMtx;
    n = obj->parts;
    if (w == NULL) {
        return;
    }
    for (i = 0; i < obj->partCount; n++, w++, i++) {
        if (n->attr & 0x80) {
            s = n->gaObjs;
            for (j = 0; j < n->gaObjCount; j++, s++) {
                if (s->clip != 0xFF) {
                    s->envUv = (void *)GsPacketAlloc64(s->vtxCount * 8 + 0x40);
                    MemMove128(s->envUv, s->uvs, 1);
                    if (s->envUv != NULL) {
                        GphGaObjEnvMapCalcUv(s, w);
                    }
                }
            }
        }
    }
}
