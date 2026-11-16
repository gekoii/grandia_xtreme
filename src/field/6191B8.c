#include "common.h"
#include "sys.h"



s32 FldBattleSetSpecialParams(void) {
    if ((btlHandoff.enemyKind[0] == 0x51) || (btlHandoff.enemyKind[0] == 0x61) || (btlHandoff.enemyKind[0] == 0x71) || (btlHandoff.enemyKind[0] == 0x41) || (btlHandoff.enemyKind[0] == 0xA1) || (btlHandoff.enemyKind[0] == 0x2B) || (btlHandoff.enemyKind[0] == 0x151) || (btlHandoff.enemyKind[0] == 0x161) || (btlHandoff.enemyKind[0] == 0x171) || (btlHandoff.enemyKind[0] == 0x181) || (btlHandoff.enemyKind[0] == 0x25) || (btlHandoff.enemyKind[0] == 0x26) || (btlHandoff.enemyKind[0] == 0x27) || (btlHandoff.enemyKind[0] == 0x23) || (btlHandoff.enemyKind[0] == 0x24)) {
        btlHandoff.unk54 = 1;
        btlHandoff.startType = 0x18;
        btlHandoff.endFade = 0;
        btlHandoff.unk60 = 0;
        btlHandoff.encType = 0;
        btlHandoff.unk6A = 4;
    }
    if ((btlHandoff.enemyKind[0] == 0x65) || (btlHandoff.enemyKind[0] == 0x165)) {
        btlHandoff.unk54 = 2;
        btlHandoff.startType = 0x18;
        btlHandoff.endFade = 0;
        btlHandoff.encType = 0;
        btlHandoff.unk60 = 0;
        btlHandoff.unk6A = 4;
    }
    if ((btlHandoff.enemyKind[0] == 0x81) || (btlHandoff.enemyKind[0] == 0x91) || (btlHandoff.enemyKind[0] == 0x1A1)) {
        btlHandoff.unk54 = 2;
        btlHandoff.startType = 0x1A;
        btlHandoff.unk6A = 4;
        btlHandoff.endFade = 0;
        btlHandoff.unk60 = 0;
        btlHandoff.encType = 0;
    }
    if (btlHandoff.enemyKind[0] == 0x1A4) {
        btlHandoff.endFade = 0;
        btlHandoff.unk6A = 4;
        btlHandoff.unk54 = 0;
        btlHandoff.startType = 0;
        btlHandoff.unk60 = 0;
        btlHandoff.encType = 0;
    }
    if (btlHandoff.enemyKind[0] == 0xA4) {
        btlHandoff.startType = 2;
        btlHandoff.unk6A = 4;
        btlHandoff.endFade = 0;
        btlHandoff.unk54 = 0;
        btlHandoff.unk60 = 0;
        btlHandoff.encType = 0;
    }
    if (btlHandoff.enemyKind[0] == 0xC1) {
        btlHandoff.unk54 = 3;
        btlHandoff.startType = 0x1C;
        btlHandoff.unk6A = 4;
        btlHandoff.endFade = 0;
        btlHandoff.unk60 = 0;
        btlHandoff.encType = 0;
    }
    if (btlHandoff.enemyKind[0] == 0xD1) {
        btlHandoff.unk54 = 3;
        btlHandoff.startType = 0x30;
        btlHandoff.unk6A = 4;
        btlHandoff.endFade = 0;
        btlHandoff.unk60 = 0;
        btlHandoff.encType = 0;
    }
    return 0;
}
