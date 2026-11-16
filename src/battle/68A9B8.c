#include "common.h"
#include "battle/misc.h"

void PtclBtlDustCloudUpdate(PtclBtlDust *p) {
    s32 n;

    n = p->unk8 + 1;
    p->unk8 = n;
    p->unk0 = p->unk0 - 0.020833334f;
    if (n < 0x20) {
        p->unk4 = p->unk4 + 2.5f;
    }
    p->unk14 = p->unk14 + 2.0f;
}
