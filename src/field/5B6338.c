#include "common.h"
#include "protos/field.h"
#include "field/event.h"

void OccsWakeById(s16 id) {
    s32 i;

    i = 0;
    while (i < occsTable->count) {
        if (((OccsEntry *)(i * 0x98 + (u32)occsTable->items))->id == id) {
            OccsWake(i);
            break;
        }
        i++;
    }
}
