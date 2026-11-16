/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n14.h"

void ChrMotTableClear(ChrMotTable *p) {
    s32 i;

    p->count = 0;
    for (i = 0; i < 63; i++) {
        p->slots[i] = -1;
    }
}
