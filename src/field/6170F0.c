/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void FldSndBgmSetVolume(f32 vol, f32 time) {
    if (vol > 100.0f) {
        vol = 100.0f;
    }
    if (fldSnd.bgmHandle != 0xFFFFFFFF) {
        fldSnd.bgmVolume = vol;
        SndBgmFadeVolume(fldSnd.bgmHandle, (s32)time, vol);
    }
}
