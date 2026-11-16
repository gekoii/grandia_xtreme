#include "common.h"
#include "field/n24.h"

void MiniRequestEnd(void) {
    if (miniCtl->dataFile != 0 && miniCtl->state != 0) {
        miniCtl->state = 3;
    }
}
