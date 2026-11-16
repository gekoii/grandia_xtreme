#include "common.h"
#include "save.h"
#include "protos/field.h"
#include "field/n25.h"

void MiniBallSetRank(void) {
    u32 lv = PtyCharCalcLevel(0);
    u32 i;

    for (i = 0; i < miniBallData->hdr->courseCount; i++) {
        if (lv < miniBallData->courses[i].minLevel) break;
    }
    sysSaveData.miniBallRank = i;
}
