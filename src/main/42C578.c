#include "common.h"
#include "grxsys.h"
#include "main/n03.h"
#include "protos/main.h"

void McInit(void) {
    s32 i;
    s32 v;

    mcCardInfo0 = ((McPortState *(*)(s32))McGetPortState)(0); /* through a cast: the direct call schedules differently */
    mcCardInfo0->state = 0;
    mcCardInfo1 = ((McPortState *(*)(s32))McGetPortState)(1);
    mcCardInfo1->state = 0;
    McIconSysBuild();
    mcSaveDataSize[0] = 0x3300;
    mcSlotTable.count = 8;
    for (i = 7; i >= 0; i--) {
        mcSlotTable.slots[i].used = 0;
    }
    v = sysGrxsysPac->unk144;
    mcIconFileEntry.size = 0xB638;
    mcIconFileEntry.data = (void *)v;
}
