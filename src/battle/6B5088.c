#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"


void EnvDimForSkill(void *u, s8 kind) {
    s32 n;

    if (kind >= 16) {
        n = 0;
        kind &= 0xF;
    } else {
        n = 0x20;
    }
    if (btlDbgDimKind >= 0) {
        kind = btlDbgDimKind;
    }
    switch (kind) {
    case 1:
        EnvDimStart(u, n, 0.5f);
        break;
    case 2:
        EnvDimStart(u, n, 0.35f);
        break;
    case 3:
        EnvDimStart(u, n, 0.25f);
        break;
    default:
        EnvDimStart(u, n, 0.75f);
        break;
    }
}
