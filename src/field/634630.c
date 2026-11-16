#include "common.h"
#include "mc.h"
#include "save.h"
#include "sys.h"
#include "field/n24.h"
#include "protos/field.h"

void FldMenuSaveInit(void) {
    sysSoftResetLock = 1;
    if (sysSaveData.mainFlag - 0x3CD2 < 0x1E) {
        sysSaveData.unk20 = 1;
    }
    McSaveFlowInit();
    McGetPortState(0)->state = 0;
    McGetPortState(1)->state = 0;
    fldMenuSaveCardSlot = 0;
    fldMenuMcDlgIsOpen = 0;
}
