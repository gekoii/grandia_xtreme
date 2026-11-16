#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void FldSndBgmPlay(u32 arg0, s32 arg1) {
    s32 r;
    s32 set;

    if (arg0 != 0xFFFFFFFF) {
        set = 1;
        fldSnd.bgmId = arg0;
    } else {
        set = 0;
    }
    if (fldSnd.bgmId != 0xFFFFFFFF) {
        r = SndBgmPlay(fldSnd.bgmId, arg1);
        if (r >= 0) {
            fldSnd.bgmHandle = r;
            if (set == 1) {
                ((s32 (*)(f32, f32))FldSndBgmSetVolume)(fldSnd.bgmVolume, 100.0f);
            }
        }
    }
}
