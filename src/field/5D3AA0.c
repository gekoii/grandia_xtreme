#include "common.h"
#include "save.h"



void EvtDungeonFlagSet(u16 arg0, s32 arg1) {
    sysSaveData.dungeonFlags[arg0 >> 3] = (sysSaveData.dungeonFlags[arg0 >> 3] & ~(1 << (arg0 & 7))) | ((arg1 & 1) << (arg0 & 7));
}
