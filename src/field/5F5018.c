#include "common.h"
#include "field/n19.h"
#include "save.h"

void ItemEggChartSetKnown(s16 arg0) {
    s32 n;

    if (arg0 < 0x2D) {
        n = itemEggChartCellTable[arg0];
        sysSaveData.eggChart[n / 8] |= 1 << (n % 8);
    }
}
