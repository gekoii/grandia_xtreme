#include "common.h"
#include "protos/main.h"
#include "main/n08.h"

s32 McIsBusy(void) {
    McGetCurPort();
    if (McIsQueued() != 0) {
        return 1;
    }
    return mcSyncResult == 0;
}
