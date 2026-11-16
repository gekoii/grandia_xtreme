#include "common.h"
#include "protos/main.h"
#include "m2c_macros.h"


void GphVu1TimeoutHook(void) {
    GsDmaTimeoutHook();
}
