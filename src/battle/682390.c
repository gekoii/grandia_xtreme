#include "common.h"
#include "battle/n28.h"

void BtlSpecialIntroInit(void) {
    btlSpecialIntro.unit = NULL;
}

void BtlSpecialIntroStart(BtlUnit *u) {
    btlSpecialIntro.state = 0;
    btlSpecialIntro.unit = u;
    u->flags &= ~8u;
}
