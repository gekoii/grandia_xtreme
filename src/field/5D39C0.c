#include "common.h"
#include "save.h"



void EvtGlobalFlagSet(u16 arg0, s32 arg1) {
    sysSaveData.globalFlags[arg0 >> 3] = (sysSaveData.globalFlags[arg0 >> 3] & ~(1 << (arg0 & 7))) | ((arg1 & 1) << (arg0 & 7));
}
