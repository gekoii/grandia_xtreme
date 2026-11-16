/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "main/n08.h"

void GphFbRippleDraw(void);

void GphScreenFxDraw(GphScreenFx *e, s32 arg1) {
    if (e == NULL) {
        return;
    }
    if (e->flags & GPH_SFX_STILL) {
        GphFbOverlayImage(0, 0, 0x280, 0x1C0, 0x7F, arg1);
    }
    if ((e->flags & (GPH_SFX_BLURFIX | GPH_SFX_BLUR)) && e->blur >= 0.1f) {
        GphFbBlur(0, 0, 0x280, 0x1C0, 0, (u8)(s32)e->blur);
    }
    if (e->flags & GPH_SFX_BOKEH) {
        GphFbBokeh(0, 0, 0x280, 0x1C0, e->bokeh);
    }
    if (e->flags & GPH_SFX_WARP) {
        GphFbMeshWarp(0, 0, 0x280, 0x1C0, e->warp);
    }
    if (e->flags & GPH_SFX_RIPPLE) {
        GphFbRippleDraw();
    }
    if (e->flags & GPH_SFX_FADE) {
        GphFbOverlayImage(0, 0, 0x280, 0x1C0, (u8)(s32)e->fadeAlpha, arg1);
    }
}
