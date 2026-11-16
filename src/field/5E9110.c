#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n12.h"


void ChrInstBuildDrawList(void) {
    s32 i, j;

    GphDrawListSet(fldInstDrawList);
    GphDrawListClear(0);
    if (chrInstPool->count != 0) {
        for (i = 0; i < 48; i++) {
            for (j = 0; j < 32; j++) {
                if (chrInstPool->slots[i][j].used != 0 && chrInstPool->slots[i][j].visible != 0) {
                    if (chrInstPool->slots[i][j].blink == 0) {
                        GphModelCalcAndDraw(chrInstPool->slots[i][j].model);
                    } else if ((u8)(chrInstPool->slots[i][j].blink % 7) != 0) {
                        if ((u8)(chrInstPool->slots[i][j].blink % 7) >= 3) {
                            GphModelCalcAndDraw(chrInstPool->slots[i][j].model);
                        }
                    }
                }
            }
        }
        GphDrawListSort();
        GphDrawListEnvMapCalcUv();
    }
}
