#include "common.h"
#include "gph.h"
#include "protos/field.h"

typedef struct {
    u32 w;
    u32 h;
    u32 tw;
    u32 th;
    u32 psm;
    u32 pad14;
    s32 size;
} TexInfo;

typedef struct {
    u8 pad0[0x30];
    u64 tex0;
} TexReg;

extern void GsDrawEnvPacketSet(void *, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32);
extern void GsRenderStatePacketSet(void *, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s64);
extern void WmapWarpSendGrid(void);
extern TexInfo wmapWarpTex;
extern u8 wmapWarpDrawEnv[];
extern u8 wmapWarpRenderState[];
extern TexReg wmapWarpTexPacket;

void WmapWarpDraw(u8 n) {
    u32 w;
    u32 h;
    s32 tw;
    s32 th;
    u32 p;
    s32 i;
    s32 y;

    tw = 0xFF;
    th = 0xFF;
    w = 640;
    h = 448;
    y = 0;
    p = 1;
    i = 0;
    for (;;) {
        i++;
        p <<= 1;
        if (i >= 11) break;
        if (p >= 640) {
            tw = i;
            w = p;
            break;
        }
    }
    p = 1;
    i = 0;
    for (;;) {
        i++;
        p <<= 1;
        if (i >= 11) break;
        if (p >= 448) {
            th = i;
            h = p;
            break;
        }
    }
    wmapWarpTex.w = w;
    wmapWarpTex.tw = tw;
    wmapWarpTex.th = th;
    wmapWarpTex.h = h;
    wmapWarpTex.size = w * h * 4;
    GphFbSwapDrawZBuf();
    GsDrawEnvPacketSet(wmapWarpDrawEnv, gsDrawFrameAddr, 0x280, 0, 0, gsZBufAddr, 0, 1, 0, 0, 639, 447, 0, 0, 0);
    GsRenderStatePacketSet(wmapWarpRenderState, 1, 1, 0, 0, 0, 0, 0,
                  0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1,
                  0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsTexLoadPacket(&wmapWarpTexPacket, &wmapWarpTex, gsZBufAddr, 0, 0, 1, 1, 0, 0, 0, 0, 0);
    wmapWarpTexPacket.tex0 = (u64)((u32)(gsZBufAddr + y * 640) >> 6) + ((u64)wmapWarpTex.tw << 26) + 0x28000 + ((u64)wmapWarpTex.psm << 20) + ((u64)wmapWarpTex.th << 30) + ((u64)1 << 34);
    GsDrawEnvPacketSend(wmapWarpDrawEnv);
    GsRenderStatePacketSend(wmapWarpRenderState);
    GsTexPacketSend(&wmapWarpTexPacket);
    WmapWarpSendGrid();
}
