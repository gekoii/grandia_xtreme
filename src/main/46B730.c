#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

/* Overlay timings of playlist types 0-5 (src/main/data/49D6A8.c). */
typedef struct MovOverlayParam {
    s32 endFrame;
    s32 scrollHeight;
    s32 stripLength;
    s32 scrollStart;
    f32 scrollSpeed;
    s32 stripStart;
    f32 stripSpeed;
} MovOverlayParam; /* size = 0x1C */

extern MovOverlayParam movOverlayParamTable[];
extern s32 movPktStripR;
extern s32 movPktStripL;
extern s32 movPktShade;
extern s32 movPktScroll;

#define UNCACHED(x) (((u32)(x) & 0x0FFFFFFF) | 0x20000000)
#define PHYS(x) ((u32)(x) & 0x0FFFFFFF)
#define PK(idx) (b->v[h % 2] + (idx))
#define T (movOverlayParamTable[movListType])

s32 MovListVblankHandler(void) {
    VoTag *b;
    s32 h;
    s32 a1;
    s32 a2;
    s32 t;
    s32 o;
    s32 p;
    s32 q;
    s32 w;
    s32 x;
    s32 c;
    s32 y;
    u32 n;

    movOddField = (*(volatile u64 *)0x12001000 >> 13) & 1;
    if (movDispActive != 0) {
        movVblankCount++;
        movPathBusy = sceGsSyncPath(1, 0);
        if (movPathBusy == 0) {
            b = voBufGetTag(&movVoBuf);
            if (b == NULL) {
                movFrameStarve++;
                movFrameMissCount++;
            } else {
                if (!(movOvlFieldCount & 1)) {
                    h = movOvlFieldCount / 2;
                    if (movOvlFieldCount >= T.stripStart) {
                        f32 d = movOvlStripStep * T.stripSpeed;
                        t = d;
                        a1 = d * 16.0f;
                        if (t <= (T.stripLength / 26 + 2) * 0x280) {
                            n = movTrpStrip - 0x2080;
                            n += (t / 640) * 0x2080;
                            n = PHYS(UNCACHED(n));
                            PK(movPktStripSrc)[1] = (PK(movPktStripSrc)[1] & 0x80000000) | (n & 0xFFFFFFF0);
                        } else {
                            n = movTrpStrip;
                            n = PHYS(UNCACHED(n));
                            PK(movPktStripSrc)[1] = (PK(movPktStripSrc)[1] & 0x80000000) | (n & 0xFFFFFFF0);
                        }
                        if (t < 0x280) {
                            x = 0x280 - t;
                        } else {
                            x = 0;
                        }
                        p = 0x18E;
                        if (h & 1) {
                            p += 0x1C0;
                        }
                        PK(movPktShade)[0] = (p << 20) | (x << 4);
                        if (t >= (T.stripLength / 26 + 1) * 0x280) { y = 0; } else if (t >= T.stripLength / 26 * 0x280) { c = t % 640; y = 0x280 - c; } else { y = 0x280; }
                        q = 0x1AC;
                        if (h & 1) {
                            q += 0x1C0;
                        }
                        PK(movPktShade)[8] = (q << 20) | (y << 4);
                        w = a1 % 0x2800;
                        PK(movPktStripL)[0] = (PK(movPktStripL)[0] & 0x3FFF0000) | w;
                        p = 0x18F;
                        if (h & 1) {
                            p += 0x1C0;
                        }
                        PK(movPktStripL)[4] = p << 20;
                        PK(movPktStripL)[8] = (PK(movPktStripL)[8] & 0x3FFF0000) | 0x2800;
                        w = 0x280;
                        x = 0; if (t >= 0x280) { if (t < (T.stripLength / 26 + 1) * w) { x = 0x2800 - a1 % 0x2800; } }
                        p = 0x1AA;
                        if (h & 1) {
                            p += 0x1C0;
                        }
                        PK(movPktStripL)[12] = (p << 20) | x;
                        PK(movPktStripR)[0] = 0x01A00000;
                        c = a1 % 0x2800;
                        PK(movPktStripR)[8] = c | 0x03400000;
                        if (t >= T.stripLength / 26 * 0x280) {
                            y = 0x2800;
                        } else {
                            y = 0x2800 - a1 % 0x2800;
                        }
                        q = 0x18F;
                        if (h & 1) {
                            q += 0x1C0;
                        }
                        PK(movPktStripR)[4] = (q << 20) | y;
                        q = 0x1AA;
                        if (h & 1) {
                            q += 0x1C0;
                        }
                        PK(movPktStripR)[12] = (q << 20) | 0x2800;
                        movOvlStripStep++;
                    } else {
                        PK(movPktShade)[0] = (PK(movPktShade)[0] & 0xFFFF0000) | 0x2800;
                        PK(movPktShade)[8] = (PK(movPktShade)[8] & 0xFFFF0000) | 0x2800;
                        PK(movPktStripL)[0] = PK(movPktStripL)[0] & 0x3FFF0000;
                        PK(movPktStripL)[8] = (PK(movPktStripL)[8] & 0x3FFF0000) | 0x10;
                        PK(movPktStripL)[4] = PK(movPktStripL)[4] & 0xFFFF0000;
                        PK(movPktStripL)[12] = PK(movPktStripL)[12] & 0xFFFF0000;
                        PK(movPktStripR)[8] = 0x10;
                        PK(movPktStripR)[0] = 0;
                        PK(movPktStripR)[4] = (PK(movPktStripR)[4] & 0xFFFF0000) | 0x2800;
                        PK(movPktStripR)[12] = (PK(movPktStripR)[12] & 0xFFFF0000) | 0x2800;
                    }
                    if (movOvlFieldCount >= T.scrollStart) {
                        f32 d = movOvlScrollStep * T.scrollSpeed;
                        t = d;
                        a2 = d * 16.0f;
                        o = a2 - 0x600; if (t <= 0x5F) { o = 0; }
                        PK(movPktScroll)[0] = o << 16;
                        o = (t >= T.scrollHeight) ? T.scrollHeight * 16 : a2;
                        PK(movPktScroll)[8] = (o << 16) | 0x1F00;
                        if (t < 0x60) { p = 0x1A00 - a2; } else { p = 0x1400; }
                        if (h & 1) { p += 0x1C00; }
                        PK(movPktScroll)[4] = (PK(movPktScroll)[4] & 0xFFFF) | (p << 16);
                        if (t >= T.scrollHeight + 0x60) { q = 0x1400; } else if (t < T.scrollHeight) { q = 0x1A00; } else { q = 0x1A00 - (a2 - T.scrollHeight * 16); }
                        if (h & 1) { q += 0x1C00; }
                        PK(movPktScroll)[12] = (PK(movPktScroll)[12] & 0xFFFF) | (q << 16);
                        movOvlScrollStep++;
                    } else {
                        p = 0x140;
                        if (h & 1) {
                            p += 0x1C0;
                        }
                        q = 0x140;
                        if (h & 1) {
                            q += 0x1C0;
                        }
                        PK(movPktScroll)[8] = 0x06001F00;
                        PK(movPktScroll)[4] = (PK(movPktScroll)[4] & 0xFFFF) | (p << 20);
                        PK(movPktScroll)[12] = (PK(movPktScroll)[12] & 0xFFFF) | (q << 20);
                        PK(movPktScroll)[0] = 0;
                    }
                    if (h >= T.endFrame || movSkipRequest != 0) {
                        if (movSkipFadeStep >= 8) {
                            movFadeDone = 1;
                            c = 0;
                        } else {
                            c = (7 - movSkipFadeStep) * 16;
                        }
                        PK(movPktFadeAlpha)[1] = c;
                        movSkipFadeStep++;
                    } else {
                        PK(movPktFadeAlpha)[1] = 0x80;
                    }
                    MovDispSetBuffer(&movDisp, (h + 1) % 2);
                    MovGifSendChain((u32)b->v[h % 2]);
                    b->status = 1;
                } else {
                    b->status = 0;
                    movFrameSent = 1;
                }
                movFrameStarve = 0;
                movOvlFieldCount++;
            }
            SndUpdate();
        }
    }
    return 0;
}
