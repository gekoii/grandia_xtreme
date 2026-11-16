#include "common.h"
#include "main/n08.h"

void McSetPollInterval(s32 arg0) {
    mcPollInterval = arg0;
}

void McSelectPort(s32 arg0) {
    if (arg0 == 0) {
        mcCurPort = mcPortState;
        return;
    }
    mcCurPort = &mcPortState1;
}
