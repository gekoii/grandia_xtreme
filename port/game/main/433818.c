/* Port-only C for GphModelDrawPartsCulled (0x00433818), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "protos/main.h"

s32 GphSphereClipTest(f32 (*m)[4], f32 *v);
void GphModelDrawPartsCulled(GphPart *part, u16 count);

/* GphModelDrawParts for models that cull every part (flags 0x80): the
   sphere test always runs (after any billboard), and a skinned part
   (drawFlags bit 0) is drawn whatever it says */
void GphModelDrawPartsCulled(GphPart *part, u16 count) {
    s32 n = count;
    s32 mtx = (s32)(*(GphModel **)0x70002800)->viewMtx;
    s32 r;
    u32 bb;

    do {
        part->drawn = 0;
        if (!(part->flags & 8) && part->gaObjs != NULL) {
            bb = part->attr & 0xE;
            if (bb != 0) {
                GphPartBillboard((bb >> 1) - 1, (f32 *)mtx);
            }
            r = GphSphereClipTest((f32 (*)[4])mtx, part->cullSphere);
            if (part->drawFlags & 1) {
                GphModelPartDrawCulled(part, mtx);
            } else if (r == 0) {
                *(s32 *)0x70002804 = 1;
                GphModelPartDrawCulled(part, mtx);
                *(s32 *)0x70002804 = 0;
            } else if (r > 0) {
                GphModelPartDrawCulled(part, mtx);
            }
        }
        part++;
        n--;
        mtx += 0x40;
    } while (n > 0);
}
