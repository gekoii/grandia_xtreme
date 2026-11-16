#include "common.h"
#include "gph.h"



static inline GphTexSlot *get_entry(u8 i) {
    if (i == 0) {
        i = 1;
    }
    return (GphTexSlot *)((u8 *)gphTexTable + i * sizeof(GphTexSlot) + 0x40);
}

GphTex *GphTexGetSource(u8 arg0) {
    return get_entry(arg0)->src;
}
