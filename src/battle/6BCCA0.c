#include "common.h"
#include "protos/battle.h"

extern char btlStrPartyFile[];
extern char btlStrEnemyFile[];

void BtlCharFileName(char *buf, s32 n, s32 x) {
    if (n > 0x1000) {
        sprintf(buf, btlStrPartyFile, n - 0x1000, x);
        return;
    }
    sprintf(buf, btlStrEnemyFile, n, x);
}
