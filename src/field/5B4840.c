#include "common.h"
#include "protos/field.h"
#include "field/event.h"

/* One parameter: 0x005B48A0 also passes the entry, through a cast; 0x005FCE58 passes only the id. */
void OccWakeById(s16 id) {
    s32 i;

    i = 0;
    while (i < occTable->count) {
        if (((OccEntry *)((i << 7) + (u32)occTable->items))->id == id) {
            OccWake(i);
            break;
        }
        i++;
    }
}
