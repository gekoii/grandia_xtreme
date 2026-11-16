#include "common.h"
#include "protos/field.h"
#include "field/event.h"

void OccLoad(OccEntry *src, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        *OccGet(i) = src[i];
    }
    OccSetCount(n);
}
