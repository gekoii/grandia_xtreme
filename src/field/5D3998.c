#include "common.h"
#include "save.h"



s32 EvtGlobalFlagGet(u16 arg0) {
    return (sysSaveData.globalFlags[arg0 >> 3] >> (arg0 & 7)) & 1;
}
