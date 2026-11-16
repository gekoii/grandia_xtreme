/* rodata: 0x007033D8-0x007033FC */
#include "common.h"
#include "protos/viewer.h"
#include "viewer/n35.h"

typedef struct {
    s32 f0;
    u8 pad4[0x3C];
    s32 f40;
    u8 pad44[0xBC];
} Sprite;

void GsTexUpload(void *);

void VwrAnimDebugDraw(void) {
    Sprite spr;

    if (vwrAnimDebugOn != 0) {
        spr.f0 = 2;
        GphAnimBuildSprite(&vwrAnim, &spr, 0x80, 0x80, 0.0f);
        spr.f40 = 0;
        DbgPrintf(4, 4, "anm %d sync %d key %d frame %.3f\n", vwrAnim.cell, vwrAnim.rep,
                      vwrAnim.key, vwrAnim.time);
        GsTexUpload(vwrAnimTex);
        GsTexPacketSend(vwrAnimTex);
        GphBatchDrawQuads(&spr);
    }
}
