#include "common.h"
#include "protos/battle.h"
#include "battle/n28.h"


s32 FxHomingPoolInit(FxHomingPool *p, s32 n) {
    p->entries = MemAlloc16C(n * 0x30);
    if (p->entries == NULL) {
        return 0;
    }
    p->count = n;
    p->speedMax = 70.0f;
    p->speedMin = 4.0f;
    p->accel = 1.0f;
    p->turnMax = 0x3000;
    p->unk18 = 100.0f;
    p->turnMin = 0x100;
    p->turnStep = 0xA00;
    FxHomingPoolClear(p);
    return 1;
}
