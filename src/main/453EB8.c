/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "main/n08.h"

void MemSet(void *, s32, s32);

void GphScreenFxSet(GphScreenFx *e, u8 type, s32 on, u32 frames, f32 val) {
    if (e == NULL) {
        return;
    }
    switch (type) {
    case 0:
        MemSet(e, 0, 0x58);
        break;
    case 1:
        if (on == 1) {
            e->flags |= GPH_SFX_BLUR;
            if (frames != 0) {
                e->blurTo = val;
                e->blurT = 0.0f;
                e->blurStep = 1.0f / (f32)frames;
            } else {
                e->blurFrom = val;
                e->blurT = 1.0f;
                e->blurTo = val;
                e->blurStep = 1.0f;
            }
        } else if (e->flags & GPH_SFX_BLUR) {
            e->flags &= 0xFFFFFFFD;
        }
        break;
    case 2:
        if (on == 1) {
            e->flags |= GPH_SFX_BOKEH;
            if (val > 0.0f) {
                e->bokehTo = val;
                e->bokehT = 0.0f;
                e->bokehStep = 1.0f / (f32)frames;
            } else {
                e->bokehFrom = 0.0f;
                e->bokehTo = 0.0f;
                e->bokehT = 0.0f;
                e->bokehStep = 0.0f;
            }
        } else if (e->flags & GPH_SFX_BOKEH) {
            e->flags &= 0xFFFFFFFB;
        }
        break;
    case 3:
        if (on == 1) {
            e->flags |= GPH_SFX_WARP;
            if (val != 0.0f) {
                e->warpTo = val;
                e->warpT = 0.0f;
                e->warpStep = 1.0f / (f32)frames;
            } else {
                e->warpFrom = 0.0f;
                e->warpT = 1.0f;
                e->warpTo = 0.0f;
                e->warpStep = 1.0f;
            }
        } else if (e->flags & GPH_SFX_WARP) {
            e->flags &= 0xFFFFFFF7;
        }
        break;
    case 4:
        if (on == 1) {
            e->fadeT = 0.0f;
            e->flags |= GPH_SFX_FADE;
            if (frames == 0) {
                e->fadeStep = 0.0f;
            } else {
                e->fadeStep = 1.0f / (f32)frames;
            }
            if (e->flags & GPH_SFX_STILL) {
                e->flags ^= GPH_SFX_STILL;
            }
        } else {
            if (e->flags & GPH_SFX_FADE) {
                e->flags &= 0xFFFFFFEF;
            }
            e->fadeT = 0.0f;
            e->fadeStep = 0.0f;
        }
        break;
    case 5:
        if (on == 1) {
            e->rippleAngle = 0.0f;
            e->flags |= GPH_SFX_RIPPLE;
        } else {
            if (e->flags & GPH_SFX_RIPPLE) {
                e->flags &= 0xFFFFFDFF;
            }
            e->rippleAngle = 0.0f;
        }
        break;
    case 6:
        if (on == 1) {
            e->flags |= GPH_SFX_STILL;
        } else if (e->flags & GPH_SFX_STILL) {
            e->flags ^= GPH_SFX_STILL;
        }
        break;
    case 7:
        if (on == 1) {
            e->flags |= GPH_SFX_BLURFIX;
            e->blur = frames;
        } else if (e->flags & GPH_SFX_BLURFIX) {
            e->blur = 0.0f;
            e->flags ^= GPH_SFX_BLURFIX;
        }
        break;
    }
}
