#include "common.h"
#include "save.h"



s32 EvtSystemFlagGet(u16 arg0) {
    return (sysSaveData.systemFlags[arg0 >> 3] >> (arg0 & 7)) & 1;
}
