#include "common.h"
#include "protos/main.h"


void SysMfifoStart(void) {
    GsMfifoInit(0x100000, 0x100000);
}
