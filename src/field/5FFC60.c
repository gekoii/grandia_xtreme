#include "common.h"
#include "save.h"



void MapMiniExploredSet(s32 arg0, u16 arg1, s32 arg2) {
    SysSaveData *b = &sysSaveData; u8 *base = b->miniMapExplored[0]; u8 *p = ((arg1 >> 3) + (arg0 << 5)) + base;
    *p = (*p & ~(1 << (arg1 & 7))) | ((arg2 & 1) << (arg1 & 7));
}
