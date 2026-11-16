#include "common.h"
#include "field/n21.h"

void FldThreatGaugeReset(void) {
    if (fldThreatGauge != NULL) {
        fldThreatGauge->showDelay = 0;
        fldThreatGauge->fadeIn = 0;
    }
}
