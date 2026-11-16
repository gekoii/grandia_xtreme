#include "common.h"
#include "save.h"



s32 EvtDungeonFlagGet(u16 arg0) {
    return (sysSaveData.dungeonFlags[arg0 >> 3] >> (arg0 & 7)) & 1;
}
