/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "protos/main.h"
#include "gph.h"

void VuVecCopy(f32 *, f32 *);

void GphShatterScatterMode0(void) {
    s32 i;
    GphPart *src;
    f32 f;

    src = gphShatter.model->parts;
    for (i = 0; i < gphShatter.model->partCount; i++) {
            VuVecCopy(gphShatterPieces[i].pos, src->pos);
            gphShatterPieces[i].rot[2] = 0;
            gphShatterPieces[i].rot[0] = 0;
            gphShatterPieces[i].rot[1] = 0;
            gphShatterPieces[i].pos[2] += 57.0f;
            gphShatterPieces[i].vel[0] = VuRandSigned(1.5f);
            gphShatterPieces[i].vel[1] = VuRandSigned(1.5f);
            gphShatterPieces[i].vel[2] = VuRandSigned(1.5f);
            gphShatterPieces[i].rotVel[0] = VuRandInt(0x400);
            gphShatterPieces[i].rotVel[1] = VuRandInt(0x400);
            gphShatterPieces[i].rotVel[2] = VuRandInt(0x400);
            f = VuRandSigned(0.5f);
            gphShatterPieces[i].unk40 = f;
            if (f > 0.0f) {
                gphShatterPieces[i].unk40 = -f;
            }
            src++;
    }
}
