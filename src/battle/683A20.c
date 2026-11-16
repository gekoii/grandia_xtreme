/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "gph.h"
#include "battle/n28.h"

void ChrPartDispInit(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 16; j++) {
            chrPartDispTable[i].ids[j] = 0;
        }
    }
}
