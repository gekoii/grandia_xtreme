#include "common.h"
#include "battle/n30.h"

void FxWeaponAuraSetSegment(BtlUnit *arg0, f32 *a, f32 *b) {
    FxWeaponAura *p = &fxWeaponAura[arg0->id];
    ChrModelData *m;
    f32 s;

    if (p->kind > 0) {
        s = 1.0f;
        m = arg0->def->modelPack;
        if (m != NULL) {
            s = m->unk5C;
        }
        p->seg0[0] = a[0] * s;
        p->seg0[1] = a[1] * s;
        p->seg0[2] = a[2] * s;
        p->seg1[0] = b[0] * s;
        p->seg1[1] = b[1] * s;
        p->seg1[2] = b[2] * s;
    }
}
