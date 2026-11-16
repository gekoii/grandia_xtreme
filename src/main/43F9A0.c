#include "common.h"
#include "gph.h"

s32 GphTexGetSourceRaw(u8 arg0) {
    return (s32)gphTexTable->slots[arg0].src;
}
