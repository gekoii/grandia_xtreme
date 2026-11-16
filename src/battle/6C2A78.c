#include "common.h"
#include "protos/battle.h"


void AiTacticAttack(void *arg0) {
    BtlUnitSelectTarget(arg0, 1, 0x107F);
    AiCmdAttack(arg0);
}
