#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

void MovDispStart(s32 arg0) {
    do {
    } while (sceGsSyncV(0) == arg0);
    movFrameMissCount = 0;
    movDispActive = 1;
    movVblankCount = 0;
}
