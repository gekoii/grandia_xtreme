/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "protos/main.h"
#include "gph.h"

void VuVecCopy(f32 *, f32 *);

void GphShatterScatterMode3(void) {
    s32 i;
    GphPart *p;
    f32 f;

    p = gphShatter.model->parts;
    for (i = 0; i < gphShatter.model->partCount; i++) {
        {
            VuVecCopy(gphShatterPieces[i].pos, p->pos);
            gphShatterPieces[i].vel[0] = 0.0f;
            gphShatterPieces[i].rot[0] = 0;
            gphShatterPieces[i].rot[1] = 0;
            gphShatterPieces[i].rot[2] = 0;
            gphShatterPieces[i].pos[2] += 57.0f;
            f = VuRandSigned(0.25f) + -0.4f;
            gphShatterPieces[i].vel[2] = 0.0f;
            gphShatterPieces[i].vel[1] = f;
            gphShatterPieces[i].rotVel[0] = VuRandInt(0x190) + 0x320;
            gphShatterPieces[i].rotVel[1] = VuRandInt(0x64) + 0x64;
            gphShatterPieces[i].rotVel[2] = VuRandInt(0x64) + 0x64;
            gphShatterPieces[i].unk40 = VuRandSigned(0.5f);
            if (gphShatterPieces[i].unk40 > 0.0f) {
                gphShatterPieces[i].unk40 = -gphShatterPieces[i].unk40;
            }
            p++;
        }
    }
}
