#include "common.h"
#include "sys.h"
#include "protos/field.h"

void FldMenuLoadExit(void) {
    FldMenuCursorReset();
    sysSoftResetLock = 0;
}
