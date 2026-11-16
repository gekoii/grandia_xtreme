#include "common.h"
#include "protos/main.h"

void GsMfifoSend(void *, s32);

void GsMfifoFlush(void) {
    GsFromSprDmaWait();
    *(s64 *)0x70000800 = 0x30000000;
    *(s64 *)0x70000808 = 0;
    GsMfifoSend((void *)0x70000800, 1);
    GsFromSprDmaWait();
    do {

    } while (GsMfifoIsBusy() != 0);
}

void GsMfifoTerm(void) {
    GsMfifoFlush();
    GsMfifoStop();
}
