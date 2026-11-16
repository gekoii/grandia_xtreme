#include "common.h"
#include "save.h"
#include "protos/field.h"
#include "field/n25.h"

void *FldMemAlloc(s32);

void MiniBallAlloc(void) {
    MiniBallData **p;
    MiniBallWork **q;

    q = &miniBallWork;
    p = &miniBallData;
    *(s32 *)&miniBallLayout = (s32)MiniGetLayout(); /* s32 stores: the pointer stores schedule differently */
    *(s32 *)&miniBallUi = (s32)MiniGetUi();
    *p = FldMemAlloc(0x44);
    *q = FldMemAlloc(0x1FD0);
    sysSaveData.unk1F69 = 0x48;
    sysSaveData.unk1F6A = 1;
}
