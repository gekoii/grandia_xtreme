/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "chr.h"

void BtlChrModelScaleGaParam30(GphModel *m, ChrModelData *s, f32 k) {
    s32 i;
    s32 j;
    GphPart *d;
    GphPart *e;
    GphGaObject *dk;
    GphGaObject *sk;

    if (m != NULL && s != NULL) {
        d = m->parts;
        e = s->parts;
        if (m->partCount == s->partCount) {
            if (k <= 0.001f) {
                k = 0.001f;
            }
            for (i = 0; i < m->partCount; i++, d++, e++) {
                dk = d->gaObjs;
                sk = e->gaObjs;
                for (j = 0; j < d->gaObjCount; j++, dk++, sk++) {
                    if (!(sk->unk30 <= 0.0f)) {
                        dk->unk30 = sk->unk30 * k;
                    }
                }
            }
        }
    }
}
