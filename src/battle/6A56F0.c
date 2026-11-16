#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "party.h"

void VuVecNormalize(f32 *, f32 *);

void FxWeaponAuraSetFromPoints(BtlUnit *u, s32 j, s8 mode, f32 scale)
{
    FVECTOR a;
    FVECTOR b;
    FVECTOR c;
    FxWeaponAura *p = &fxWeaponAura[u->id];
    PtyTraceDef *fx;
    s32 cnt;
    PtyCharDef *df;
    BtlChrData *d;

    if (p->kind > 0) {
        if (u->id < 4) {
            /* &ptyCharDefTable[id - 0x1001] written out: the plain index differs */
            df = (PtyCharDef *)(u->params->id * 0x7C + (u32)ptyCharDefTable - 0x7C07C);
            cnt = df->traceCount;
            fx = df->traces;
        } else {
            d = u->def->data;
            cnt = d->traceCount;
            fx = d->traces;
        }
        if (j < cnt) {
            fx += j;
            p->unk8 = fx->parts[0];
            p->unkA = fx->parts[1];
            a[0] = fx->offset0[0];
            a[1] = fx->offset0[1];
            a[2] = fx->offset0[2];
            if (fx->parts[0] == fx->parts[1]) {
                b[0] = fx->offset1[0] - fx->offset0[0];
                b[1] = fx->offset1[1] - fx->offset0[1];
                b[2] = fx->offset1[2] - fx->offset0[2];
                if (mode == 1) {
                    VuVecNormalize(c, b);
                    __asm__ volatile(
                        "lqc2 $vf1, 0(%0)\n\t"
                        "qmtc2.ni %1, $vf2\n\t"
                        "vmulx.xyz $vf1, $vf1, $vf2x\n\t"
                        "sqc2 $vf1, 0(%0)"
                        : : "r"(c), "r"(scale) : "memory");
                    FxWeaponAuraSetDir(u, 1, c[0], c[1], c[2]);
                }
            } else {
                b[0] = fx->offset1[0];
                b[1] = fx->offset1[1];
                b[2] = fx->offset1[2];
            }
            FxWeaponAuraSetSegment(u, a, b);
        }
    }
}
