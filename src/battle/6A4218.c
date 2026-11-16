#include "common.h"
#include "battle/n30.h"
#include "protos/battle.h"

void FxDefeatBurstStart(BtlUnit *arg0) {
    FxDefeat *p = &fxDefeat[arg0->id];
    FxDefeatPart *e;
    s32 i;

    if (p->parts == NULL) {
        e = (FxDefeatPart *)BtlAlloc32(0x21C);
        p->parts = e;
        if (e != NULL) {
            i = 15;
            do {
                e->life = 0;
                i--;
                e++;
            } while (i > 0);
            p->unk5 = 0;
            p->unk4 = 0;
        }
    }
}
