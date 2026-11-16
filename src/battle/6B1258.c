#include "common.h"
#include "battle/misc.h"

void MgcSwitchProc(void) {
    mgcWork.update = mgcWork.next;
}
