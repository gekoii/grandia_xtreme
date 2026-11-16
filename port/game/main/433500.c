/* Port-only C for GphModelDrawParts (0x00433500), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "protos/main.h"

s32 GphSphereClipTest(f32 (*m)[4], f32 *v);
void GphModelDrawParts(GphPart *part, u16 count);

/* Draws count parts (at least one) of the current model with its view
   matrices (*(GphModel **)0x70002800, 0x40 bytes each, one per part).
   Billboards first; unless the part is skinned (drawFlags bit 0) or the
   whole model is inside the view (*(s32 *)0x70002804 == 1 on entry), a
   part outside its cull sphere test is skipped, and one that is wholly
   inside is drawn with 0x70002804 set to 1. */
void GphModelDrawParts(GphPart *part, u16 count) {
    s32 n = count;
    s32 mtx = (s32)(*(GphModel **)0x70002800)->viewMtx;
    s32 inside = *(s32 *)0x70002804;
    s32 r;
    u32 bb;

    do {
        part->drawn = 0;
        if (!(part->flags & 8) && part->gaObjs != NULL) {
            bb = part->attr & 0xE;
            if (bb != 0) {
                GphPartBillboard((bb >> 1) - 1, (f32 *)mtx);
            }
            if ((part->drawFlags & 1) || inside == 1) {
                GphModelPartDraw(part, mtx);
            } else {
                r = GphSphereClipTest((f32 (*)[4])mtx, part->cullSphere);
                if (r == 0) {
                    *(s32 *)0x70002804 = 1;
                    GphModelPartDraw(part, mtx);
                    *(s32 *)0x70002804 = 0;
                } else if (r > 0) {
                    GphModelPartDraw(part, mtx);
                }
            }
        }
        part++;
        n--;
        mtx += 0x40;
    } while (n > 0);
}
