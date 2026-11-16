/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "gph.h"

void VuVecCopy(f32 *, f32 *);

void GphShatterScatterMode9(void) {
    GphPart *src;
    s32 i;
    f32 f;
    GphShatterPiece *e;
    s32 t;

    src = gphShatter.model->parts;
    for (i = 0; i < gphShatter.model->partCount; i++, src++) {
        {
            s32 off = 0x50;
            off = i * off;
            VuVecCopy((f32 *)((u8 *)gphShatterPieces + off), src->pos);
            e = (GphShatterPiece *)(off + (s32)gphShatterPieces);
            e->rot[2] = 0;
            e->rot[0] = 0;
            e->rot[1] = 0;
            e->pos[2] += 57.0f;
            ((GphShatterPiece *)(off + (s32)gphShatterPieces))->vel[0] = VuRandSigned(8.0f);
            t = VuRandInt(8);
            e = (GphShatterPiece *)(off + (s32)gphShatterPieces);
            e->vel[1] = t;
            e->vel[2] = 0.0f;
            ((GphShatterPiece *)(off + (s32)gphShatterPieces))->rotVel[0] = VuRandInt(0x400);
            ((GphShatterPiece *)(off + (s32)gphShatterPieces))->rotVel[1] = VuRandInt(0x400);
            ((GphShatterPiece *)(off + (s32)gphShatterPieces))->rotVel[2] = VuRandInt(0x400);
            f = VuRandSigned(1.0f);
            off += (s32)gphShatterPieces;
            e = (GphShatterPiece *)off;
            e->unk40 = f;
            if (f > 0.0f) {
                e->unk40 = -f;
            }
        }
    }
}
