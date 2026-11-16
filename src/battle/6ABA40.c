#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"


void PtclBurstFree(PtclBurst *arg0) {
    if (arg0->unk2C != NULL) {
        MemFree(arg0->unk2C);
        arg0->unk2C = NULL;
    }
    if (arg0->parts != NULL) {
        MemFree(arg0->parts);
        arg0->parts = NULL;
    }
    if (arg0->parts2 != NULL) {
        MemFree(arg0->parts2);
        arg0->parts2 = NULL;
    }
    arg0->cb38 = NULL;
    arg0->cb34 = NULL;
    arg0->cb30 = NULL;
    arg0->type = 0;
}
