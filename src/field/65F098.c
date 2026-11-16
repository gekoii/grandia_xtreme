#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McLoadSync(void) {
    if (mcLoadState == 0) {
        McScanPorts();
        return;
    }
    McUpdate();
}
