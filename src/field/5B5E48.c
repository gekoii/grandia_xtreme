#include "common.h"
#include "field/event.h"

void OccsSetCount(s32 arg0) {
    occsTable->count = arg0;
}

s32 OccsGetCount(void) {
    return occsTable->count;
}
