#include "common.h"
#include "save.h"



s32 ItemEggChartIsKnown(s16 arg0) {
    if (arg0 < 0x2B) {
        return (sysSaveData.eggChart[arg0 / 8] >> (arg0 % 8)) & 1 ? 0 : -1;
    }
    return -1;
}
