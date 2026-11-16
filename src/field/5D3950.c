#include "common.h"
#include "save.h"



void EvtSystemFlagSet(u16 arg0, s32 arg1) {
    sysSaveData.systemFlags[arg0 >> 3] = (sysSaveData.systemFlags[arg0 >> 3] & ~(1 << (arg0 & 7))) | ((arg1 & 1) << (arg0 & 7));
}
