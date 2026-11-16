#include "common.h"
#include "protos/field.h"
#include "m2c_macros.h"


void EvtStateResetAll(void) {
    EvtAreaStateReset();
    EvtMapStateReset();
}
