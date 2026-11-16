#include "common.h"
#include "gph.h"

#define FIX(f) if (h->f) h->f = (u8 *)h->f + (u32)h

void GphTexDataRelocate(GphTex *h) {
    FIX(data[0]);
    FIX(data[1]);
    FIX(data[2]);
    FIX(data[3]);
    FIX(data[4]);
    FIX(data[5]);
    FIX(data[6]);
    FIX(unk40[0]);
    FIX(unk40[1]);
    FIX(unk40[2]);
    FIX(unk40[3]);
    FIX(unk40[4]);
    FIX(unk40[5]);
    FIX(unk40[6]);
    FIX(unk40[7]);
}
