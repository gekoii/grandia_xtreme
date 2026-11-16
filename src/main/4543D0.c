/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "vu.h"
#include "main/n08.h"

void VuVecLerp(FVECTOR, FVECTOR, FVECTOR, f32);

void GphScreenFxUpdate(GphScreenFx *o) {
    FVECTOR from;
    FVECTOR to;
    FVECTOR out;

    if (o == NULL) {
        return;
    }
    if (o->flags & GPH_SFX_BLUR) {
        if (o->blurT < 1.0f) {
            from[0] = o->blurFrom;
            to[0] = o->blurTo;
            VuVecLerp(out, from, to, o->blurT);
            o->blurT += o->blurStep;
        } else {
            o->blurFrom = o->blurTo;
            out[0] = o->blurTo;
            if (!(o->flags & GPH_SFX_HOLD)) {
                o->flags ^= GPH_SFX_BLUR;
            }
        }
        o->blur = out[0];
    }
    if (o->flags & GPH_SFX_BOKEH) {
        if (o->bokehT < 1.0f) {
            from[0] = o->bokehFrom;
            to[0] = o->bokehTo;
            VuVecLerp(out, from, to, o->bokehT);
            o->bokehT += o->bokehStep;
        } else {
            o->bokehFrom = o->bokehTo;
            out[0] = o->bokehTo;
        }
        o->bokeh = out[0];
    }
    if (o->flags & GPH_SFX_WARP) {
        if (o->warpT < 1.0f) {
            from[0] = o->warpFrom;
            to[0] = o->warpTo;
            VuVecLerp(out, from, to, o->warpT);
            o->warpT += o->warpStep;
        } else {
            o->warpFrom = o->warpTo;
            out[0] = o->warpTo;
        }
        o->warp = out[0];
    }
    if (o->flags & GPH_SFX_RIPPLE) {
        GphFbRippleBuildCentered(o->rippleAngle);
        o->rippleAngle += 0.37699103f;
        if (o->rippleAngle > 62.83184f) {
            o->rippleAngle = 0.0f;
            o->flags ^= GPH_SFX_RIPPLE;
        }
    }
    if (o->flags & GPH_SFX_FADE) {
        if (o->fadeT < 1.0f) {
            from[0] = 128.0f;
            to[0] = 0.0f;
            VuVecLerp(out, from, to, o->fadeT);
            o->fadeT += o->fadeStep;
        } else if (!(o->flags & GPH_SFX_HOLD)) {
            o->flags ^= GPH_SFX_FADE;
        }
        o->fadeAlpha = out[0];
    }
}
