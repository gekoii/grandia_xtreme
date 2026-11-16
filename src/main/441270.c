/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "main/n06.h"

s32 SndBgmFadeVolume(u8 idx, s32 arg1, f32 x) {
    if (sndBgmEnabled == 0) {
        return 0;
    }
    sndBgmChannels[idx].fade = arg1;
    sndBgmChannels[idx].volume = (u8)(s32)(x / 100.0f * 255.0f);
    sndBgmChannels[idx].fadeState = 0;
    return 0;
}
