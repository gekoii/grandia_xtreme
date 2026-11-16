#include "common.h"
#include "field/n26.h"

void McSpaceCheckReset(void) {
    mcSpaceCheckState = 0;
    mcSpaceCheckPrevState = 0xFFFF;
    mcSpaceCheckStateChanged = 0;
}
