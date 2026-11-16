#include "common.h"
#include "protos/field.h"

typedef struct {
    u32 count;
    s32 offs[1];
} Hdr;


void PtclFldLoad(Hdr *arg0) {
    u32 i;

    for (i = 0; i < arg0->count; i++) {
        arg0->offs[i] += (s32)arg0;
        PtclFldEffectLoad(i, arg0->offs[i]);
    }
    PtclFldSystemCreate();
}
