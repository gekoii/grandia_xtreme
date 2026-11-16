#include "common.h"
#include "save.h"



void EvtSetMapNo(s32 arg0, s32 arg1) {
    sysSaveData.mapNo = arg0;
    sysSaveData.oldMapNo = arg1;
}
