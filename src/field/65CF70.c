#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McSaveSync(void) {
    if (mcSaveState == 0) {
        McScanPorts();
        return;
    }
    McUpdate();
}
