#include "common.h"
#include "save.h"



s32 MapMiniExploredGet(s32 arg0, u16 arg1) {
    return (sysSaveData.miniMapExplored[arg0][arg1 >> 3] >> (arg1 & 7)) & 1;
}
