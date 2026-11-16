#include "common.h"
#include "save.h"



s32 EvtGameFlagGet(u16 arg0) {
    return (sysSaveData.gameFlags[arg0 >> 3] >> (arg0 & 7)) & 1;
}
