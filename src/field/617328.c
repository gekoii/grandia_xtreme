/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/state.h"
#include "protos/field.h"
extern f32 fldSndSeRange;

void FldSndSePlay(u8 grp, s32 data, void *vec) {
    s32 flags = 0;
    if (data == 0) return;
    if (vec != 0) {
        if (FldVecDist(vec, camField[0].lookAt) > fldSndSeRange) return;
        flags = 8;
    }
    SndSePlay3D(data, grp, flags, vec);
}
