#include "common.h"
#include "protos/main.h"

void GphMeshTimeoutHook(void) {
    GsDmaTimeoutHook();
}
