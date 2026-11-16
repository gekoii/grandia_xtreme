#include "common.h"
#include "protos/field.h"
#include "m2c_macros.h"


void FldDebugHook(void) {
    if ((MapLoadGetState() == -1) || (MapLoadGetState() >= 0xA)) {
        DbgFldMenu();
    }
}
