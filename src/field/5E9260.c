#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/n12.h"


void ChrInstUpdate(void) {
    s32 i, j;

    GphDrawListSet(fldInstDrawList);
    if (chrInstPool->count != 0) {
        for (i = 0; i < 48; i++) {
            for (j = 0; j < 32; j++) {
                if (chrInstPool->slots[i][j].used != 0 && chrInstPool->slots[i][j].visible != 0) {
                    if (chrInstPool->slots[i][j].spin != 0) {
                        chrInstPool->slots[i][j].model->rot[1] += 0x100;
                    }
                    ChrMotUpdate(chrInstPool->slots[i][j].model, 1.0f);
                }
            }
        }
    }
}
