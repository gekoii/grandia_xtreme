#include "common.h"
#include "protos/battle.h"
#include "battle/n28.h"


void BtlResultLoad(void *arg0) {
    BtlResult **p;

    p = &btlResult;
    UtilTexRegisterWithId(BtlPakGetEntry(arg0, 5), 0x97);
    UtilTexRegisterWithId(BtlPakGetEntry(arg0, 6), 0x98);
    UtilTexRegisterWithId(BtlPakGetEntry(arg0, 7), 0x99);
    UtilTexRegisterWithId(BtlPakGetEntry(arg0, 8), 0x9A);
    UtilTexRegisterWithId(BtlPakGetEntry(arg0, 9), 0x9B);
    UtilTexRegisterWithId(BtlPakGetEntry(arg0, 0xA), 0x9C);
    UtilTexRegisterWithId(BtlPakGetEntry(arg0, 0xB), 0x9D);
    UtilTexRegisterWithId(BtlPakGetEntry(arg0, 0xC), 0x9E);
    UtilTexRegisterWithId(BtlPakGetEntry(arg0, 0xD), 0x9F);
    UtilTexRegisterWithId(BtlPakGetEntry(arg0, 0xE), 0xA0);
    UtilTexRegisterWithId(BtlPakGetEntry(arg0, 0xF), 0xA1);
    *p = MemCalloc16A(0x20C8);
}
