#include "common.h"
#include "sys.h"
#include "protos/field.h"

void FldMenuSaveExit(void) {
    FldMenuCursorReset();
    sysSoftResetLock = 0;
}
