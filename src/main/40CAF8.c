/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "protos/main.h"
#include "gph.h"

void VuVecCopy(f32 *, f32 *);

void GphShatterScatterMode1(void) {
    s32 i;
    GphPart *p;
    f32 f;

    p = gphShatter.model->parts;
    for (i = 0; i < gphShatter.model->partCount; i++) {
        VuVecCopy(gphShatterPieces[i].pos, p->pos);
        gphShatterPieces[i].rot[2] = 0;
        gphShatterPieces[i].rot[0] = 0;
        gphShatterPieces[i].rot[1] = 0;
        gphShatterPieces[i].pos[2] += 57.0f;
        gphShatterPieces[i].vel[0] = VuRandSigned(1.0f);
        gphShatterPieces[i].vel[1] = VuRandSigned(1.0f);
        gphShatterPieces[i].vel[2] = VuRandSigned(1.0f);
        gphShatterPieces[i].rotVel[0] = VuRandInt(800);
        gphShatterPieces[i].rotVel[1] = VuRandInt(800);
        gphShatterPieces[i].rotVel[2] = VuRandInt(800);
        f = VuRandSigned(0.4f);
        gphShatterPieces[i].unk40 = f;
        if (f > 0.0f) {
            gphShatterPieces[i].unk40 = -f;
        }
        p++;
    }
}
