/* rodata: 0x004A6C48-0x004A6C60 */
#include "common.h"
#include "protos/main.h"
#include "main/n11.h"


#define UNCACHED(x) (((u32)(x) & 0x0FFFFFFF) | 0x20000000)
#define PHYS(x) ((u32)(x) & 0x0FFFFFFF)
#define PK(idx) (b->v[h % 2] + (idx))

s32 MovIpartVblankHandler(void) {
    VoTag *b;
    s32 h;
    s32 a1;
    s32 a2;
    s32 t;
    s32 k;
    s32 m;
    s32 o;
    s32 p;
    s32 q;
    s32 w;
    u32 n;
    double d;

    movOddField = (*(volatile u64 *)0x12001000 >> 13) & 1;
    if (movDispActive != 0) {
        movVblankCount++;
        movPathBusy = sceGsSyncPath(1, 0);
        if (movPathBusy == 0) {
            b = voBufGetTag(&movVoBuf);
            if (b == NULL && movOvlFieldCount < 0x1EA8) {
                movFrameMissCount++;
            } else {
                if (movOvlFieldCount >= 0x1EA8) {
                    b = movVoBuf.tag[0];
                    movOvlEnd = 1;
                }
                if (!(movOvlFieldCount & 1)) {
                    h = movOvlFieldCount / 2;
                    if (movOvlFieldCount >= 0x1F58) {
                        d = movOvlStripStep * 4.7;
                        a1 = d * 16.0;
                        t = d;
                        if (t <= 0x4100) {
                            n = movTrpStrip - 0x2080;
                            n += (t / 640) * 0x2080;
                            n = PHYS(UNCACHED(n));
                            PK(movPktStripSrc)[1] = (PK(movPktStripSrc)[1] & 0x80000000) | (n & 0xFFFFFFF0);
                        } else {
                            n = movTrpStrip;
                            n = PHYS(UNCACHED(n));
                            PK(movPktStripSrc)[1] = (PK(movPktStripSrc)[1] & 0x80000000) | (n & 0xFFFFFFF0);
                        }
                        w = a1 % 0x2800;
                        PK(movPktIpartStripL)[0] = (PK(movPktIpartStripL)[0] & 0x3FFF0000) | w;
                        p = 0x187;
                        if (h & 1) {
                            p += 0x1C0;
                        }
                        PK(movPktIpartStripL)[4] = p << 20;
                        PK(movPktIpartStripL)[8] = (PK(movPktIpartStripL)[8] & 0x3FFF0000) | 0x2800;
                        w = 0;
                        if (t >= 0x280) {
                            if (t < 0x3E80) {
                                w = 0x2800 - a1 % 0x2800;
                            }
                        }
                        p = 0x1A2;
                        if (h & 1) {
                            p += 0x1C0;
                        }
                        PK(movPktIpartStripL)[12] = (p << 20) | w;
                        PK(movPktIpartStripR)[0] = 0x01A00000;
                        m = a1 % 0x2800;
                        PK(movPktIpartStripR)[8] = m | 0x03400000;
                        if (t >= 0x3C00) {
                            p = 0x2800;
                        } else {
                            p = 0x2800 - a1 % 0x2800;
                        }
                        q = 0x187;
                        if (h & 1) {
                            q += 0x1C0;
                        }
                        PK(movPktIpartStripR)[4] = (q << 20) | p;
                        q = 0x1A2;
                        if (h & 1) {
                            q += 0x1C0;
                        }
                        PK(movPktIpartStripR)[12] = (q << 20) | 0x2800;
                        movOvlStripStep++;
                    } else {
                        PK(movPktIpartStripL)[0] = PK(movPktIpartStripL)[0] & 0x3FFF0000;
                        PK(movPktIpartStripL)[8] = (PK(movPktIpartStripL)[8] & 0x3FFF0000) | 0x10;
                        PK(movPktIpartStripL)[4] = PK(movPktIpartStripL)[4] & 0xFFFF0000;
                        PK(movPktIpartStripL)[12] = PK(movPktIpartStripL)[12] & 0xFFFF0000;
                        PK(movPktIpartStripR)[8] = 0x10;
                        PK(movPktIpartStripR)[0] = 0;
                        PK(movPktIpartStripR)[4] = (PK(movPktIpartStripR)[4] & 0xFFFF0000) | 0x2800;
                        PK(movPktIpartStripR)[12] = (PK(movPktIpartStripR)[12] & 0xFFFF0000) | 0x2800;
                    }
                    if (movOvlFieldCount >= 0x1F58) {
                        d = movOvlScrollStep * 0.32;
                        a2 = d * 16.0;
                        t = d;
                        o = a2 - 0x1680;
                        if (t <= 0x167) {
                            o = 0;
                        }
                        PK(movPktIpartColumn)[0] = o << 16;
                        o = a2;
                        if (t >= 0x2DC) {
                            o = 0x2DC0;
                        }
                        PK(movPktIpartColumn)[8] = (o << 16) | 0x1540;
                        if (t < 0x168) {
                            p = 0x1800 - a2;
                        } else {
                            p = 0x180;
                        }
                        if (h & 1) {
                            p += 0x1C00;
                        }
                        PK(movPktIpartColumn)[4] = (PK(movPktIpartColumn)[4] & 0xFFFF) | (p << 16);
                        if (t >= 0x444) {
                            q = 0x180;
                        } else if (t < 0x2DC) {
                            q = 0x1800;
                        } else {
                            q = 0x45C0 - a2;
                        }
                        if (h & 1) {
                            q += 0x1C00;
                        }
                        PK(movPktIpartColumn)[12] = (PK(movPktIpartColumn)[12] & 0xFFFF) | (q << 16);
                        movOvlScrollStep++;
                    } else {
                        p = 0x18;
                        if (h & 1) {
                            p += 0x1C0;
                        }
                        q = 0x18;
                        if (h & 1) {
                            q += 0x1C0;
                        }
                        PK(movPktIpartColumn)[8] = 0x06001540;
                        PK(movPktIpartColumn)[4] = (PK(movPktIpartColumn)[4] & 0xFFFF) | (p << 20);
                        PK(movPktIpartColumn)[12] = (PK(movPktIpartColumn)[12] & 0xFFFF) | (q << 20);
                        PK(movPktIpartColumn)[0] = 0;
                    }
                    if (movOvlFieldCount >= 0x3A46) {
                        d = movOvlPictureStep * 0.59;
                        a2 = d * 16.0;
                        t = d;
                        if (t <= 0x1C0 || t >= 0xB66) {
                            p = movTrpPicture;
                            p = PHYS(UNCACHED(p));
                            PK(movPktIpartPictureSrc)[1] = (PK(movPktIpartPictureSrc)[1] & 0x80000000) | (p & 0xFFFFFFF0);
                        } else {
                            p = movTrpPicture + (t - 0x1C0) * 0x140;
                            p = PHYS(UNCACHED(p));
                            PK(movPktIpartPictureSrc)[1] = (PK(movPktIpartPictureSrc)[1] & 0x80000000) | (p & 0xFFFFFFF0);
                        }
                        q = 0;
                        if ((u32)(t - 0x1C0) < 0x9A7) {
                            m = a2 % 16;
                        } else {
                            m = 0;
                        }
                        PK(movPktIpartPicture)[0] = (m << 16) | (q << 4);
                        m = 0x258;
                        if (t < 0x1C0) {
                            k = a2;
                        } else if (t < 0x9A6) {
                            k = a2 % 16 + 0x1C00;
                        } else if (t < 0xB66) {
                            k = (0xB66 - t) * 16 + a2 % 16;
                        } else {
                            k = 0;
                        }
                        PK(movPktIpartPicture)[8] = (k << 16) | (m << 4);
                        if (t < 0x1C0) {
                            p = 0x1C00 - a2;
                        } else {
                            p = 0;
                        }
                        if (h & 1) {
                            p += 0x1C00;
                        }
                        PK(movPktIpartPicture)[4] = (PK(movPktIpartPicture)[4] & 0xFFFF) | (p << 16);
                        if (t < 0x9A6) {
                            q = 0x1C00;
                        } else if (t < 0xB66) {
                            q = (0xB66 - t) * 16;
                        } else {
                            q = 0;
                        }
                        if (h & 1) {
                            q += 0x1C00;
                        }
                        PK(movPktIpartPicture)[12] = (PK(movPktIpartPicture)[12] & 0xFFFF) | (q << 16);
                        movOvlPictureStep++;
                    } else {
                        p = 0;
                        if (h & 1) {
                            p += 0x1C0;
                        }
                        PK(movPktIpartPicture)[0] = 0;
                        PK(movPktIpartPicture)[4] = (PK(movPktIpartPicture)[4] & 0xFFFF) | (p << 20);
                        PK(movPktIpartPicture)[8] = 0;
                        PK(movPktIpartPicture)[12] = (PK(movPktIpartPicture)[12] & 0xFFFF) | (p << 20);
                    }
                    if (movOvlFieldCount >= 0x3A26) {
                        if (movOvlDimFadeStep < 0x10) {
                            q = (0xF - movOvlDimFadeStep) * 8;
                        } else {
                            q = 0;
                        }
                        PK(movPktIpartDimAlpha)[1] = q;
                        movOvlDimFadeStep++;
                    } else {
                        PK(movPktIpartDimAlpha)[1] = 0x80;
                    }
                    if (movOvlFieldCount >= 0x60E6) {
                        if (movOvlLabelFadeStep < 0x20) {
                            q = (movOvlLabelFadeStep + 1) * 4;
                        } else {
                            q = 0x80;
                            if (movOvlLabelFadeStep >= 0xB6) {
                                if (movOvlLabelFadeStep < 0xC6) {
                                    q = (0xC5 - movOvlLabelFadeStep) * 8;
                                } else {
                                    q = 0;
                                }
                            }
                        }
                        PK(movPktIpartLabelAlpha)[1] = q;
                        movOvlLabelFadeStep++;
                    } else {
                        PK(movPktIpartLabelAlpha)[1] = 0;
                    }
                    if (h >= 0x3156 || movSkipRequest != 0) {
                        if (movSkipFadeStep >= 8) {
                            movFadeDone = 1;
                            q = 0;
                        } else {
                            q = (7 - movSkipFadeStep) * 16;
                        }
                        PK(movPktFadeAlpha)[1] = q;
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
                movOvlFieldCount++;
            }
        }
    }
    return 0;
}
