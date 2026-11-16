/* Port-only C for func_00475878 (0x00475878), which the decompilation still has in assembly. */
#include "common.h"

void func_00475878(u8 *dst, s16 *src);

/* The "put" twin of _addBlock (splat has it as data, decoded by hand):
   dst = clip(src) to 0..255 for a whole macroblock (6 blocks, 384
   samples), clipped as signed halfwords (pminh with 255, pmaxh with 0,
   then ppacb). After the loop it runs through _addBlock's constant
   D_004758C0 (0x00FF00FF words, which decode as dsra32 to $zero, no-ops)
   to its jr $ra. $v0 isn't set, so it returns nothing. */
void func_00475878(u8 *dst, s16 *src) {
    s32 i;
    s16 v;

    for (i = 0; i < 384; i++) {
        v = src[i];
        if (v > 0xFF) {
            v = 0xFF;
        }
        if (v < 0) {
            v = 0;
        }
        dst[i] = v;
    }
}
