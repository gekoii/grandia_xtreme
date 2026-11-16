#include "common.h"
#include "field/n14.h"

s32 TxtGlyphLayoutGet(s32 arg0) {
    return (s32)txtGlyphLayout[arg0 & 0xFF].sprites;
}
