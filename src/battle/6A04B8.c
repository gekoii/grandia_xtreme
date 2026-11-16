#include "common.h"
#include "battle/n30.h"

void FxUnitInit(void) {
    s32 i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13;
    FxCastAura *p0;
    FxCastSpark *p1;
    FxStatusIcon *p2;
    FxFailLabel *p4;
    FxWall *p5;
    FxWallShards *p6;
    FxRegen *p7;
    FxAntiMagic *pa;
    FxStun *ps;
    FxBind *pb;
    FxDefeat *pd;
    FxWeaponAura *p9;

    for (i1 = 0, p0 = fxCastAura; i1 < 16; i1++, p0++) {
        p0->kind = 0;
        p0->pool = 0;
    }
    for (i2 = 0, p1 = fxCastSparks; i2 < 50; i2++, p1++) {
        p1->life = 0;
    }
    fxCastSparkCount = 0;
    for (i3 = 0, p2 = fxStatusIcons[0]; i3 < 16 * 12; i3++, p2++) {
        p2->state = 0;
    }
    for (i4 = 0; i4 < 16; i4++) {
        fxStatusIconAlpha[i4] = 1.0f;
    }
    for (i5 = 0, p4 = fxFailLabel; i5 < 16; i5++, p4++) {
        p4->timer = -1;
    }
    for (i6 = 0, p5 = fxWall; i6 < 16; i6++, p5++) {
        p5->state = 0;
        p5->pool = 0;
    }
    for (i7 = 0, p6 = fxWallShards; i7 < 16; i7++, p6++) {
        p6->kind = 0;
        p6->pool = 0;
        p6->shards = NULL;
    }
    for (i8 = 0, p7 = fxRegen; i8 < 16; i8++, p7++) {
        p7->parts = NULL;
        p7->delay = 0;
    }
    for (i9 = 0, pa = fxAntiMagic; i9 < 16; i9++, pa++) {
        pa->parts = NULL;
    }
    for (i10 = 0, ps = fxStun; i10 < 16; i10++, ps++) {
        ps->parts = NULL;
    }
    for (i11 = 0, pb = fxBind; i11 < 16; i11++, pb++) {
        pb->parts = NULL;
    }
    for (i12 = 0, pd = fxDefeat; i12 < 16; i12++, pd++) {
        pd->parts = NULL;
    }
    fxComboLink.parts = NULL;
    for (i13 = 0, p9 = fxWeaponAura; i13 < 16; i13++, p9++) {
        p9->kind = 0;
    }
}
