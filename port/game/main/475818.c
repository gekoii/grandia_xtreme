/* Port-only C for _addBlock (0x00475818), which the decompilation still has in assembly. */
#include "common.h"

s32 _addBlock(u8 *dst, s16 *pred, s16 *blk);

/* MSSG mpeg2decode's Add_Block for a whole macroblock (6 blocks, 384
   samples): dst = clip(pred + blk) to 0..255, the sums in 16 bits (paddh
   wraps) and clipped as signed (pminh/pmaxh, then ppacb). Returns what the
   asm leaves in $v0, the low word of its last load from blk; the caller
   ignores it. */
s32 _addBlock(u8 *dst, s16 *pred, s16 *blk) {
    s32 i;
    s16 v;

    for (i = 0; i < 384; i++) {
        v = (s16)(pred[i] + blk[i]);
        if (v > 0xFF) {
            v = 0xFF;
        }
        if (v < 0) {
            v = 0;
        }
        dst[i] = v;
    }
    return *(s32 *)&blk[376];
}
