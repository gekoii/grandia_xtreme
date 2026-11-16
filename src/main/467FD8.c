#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

void sceGsResetPath(void);
void sceGsPutDispEnv(MovDisp *);
void sceGsDefDispEnv(MovDisp *, s32, s32, s32, s32, s32);
extern u8 movDispInitPacket[];

void MovDispInit(MovDisp *d, s32 w, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8) {
    u32 pkt;
    void *p;

    movDispActive = 0;
    movVblankCount = 0;
    movFrameSent = 0;
    movOddField = 0;
    movPathBusy = 0;
    movFadeDone = 0;
    movOvlEnd = 0;
    movFrameStarve = 0;
    movOvlFieldCount = 0;
    movOvlScrollStep = 0;
    movOvlStripStep = 0;
    movOvlPictureStep = 0;
    movOvlLabelFadeStep = 0;
    movOvlDimFadeStep = 0;
    movSkipFadeStep = 0;
    d->unk34 = a2;
    d->unk38 = a3;
    d->unk3C = a4;
    d->unk40 = a5;
    d->fbp0 = 0;
    d->width = w;
    d->fbp1 = (w + 63) / 64 * 0x7000 >> 11;
    d->unk44 = a6;
    d->imageWidth = a7;
    d->imageHeight = a8;
    sceGsDefDispEnv(d, 0, 640, 448, 0, 0);
    d->dbx = 0;
    d->dby = 0;
    d->fbw = 10;
    sceGsPutDispEnv(d);
    sceGsResetPath();
    pkt = (u32)movDispInitPacket & 0x0FFFFFFF;
    p = MovGsGifTag(pkt | 0x20000000, 0xE, 1, 0, 0, 0, 1, 7);
    p = MovGsPrmodeCont(p, 1);
    p = MovGsFrame(p, 0, (d->width + 63) / 64, 0, 0);
    p = MovGsZbuf(p, 1, 0, 0);
    p = MovGsTest(p, 0, 0, 0, 0, 0, 0, 1, 1);
    p = MovGsScissor(p, 0, d->width - 1, 0, 0x380);
    p = MovGsXyOffset(p, 0, 0);
    MovGsClamp(p, 1, 1, 0, 0, 0, 0);
    *(volatile u32 *)0x1000A010 = pkt;
    *(volatile u32 *)0x1000A020 = 8;
    *(volatile u32 *)0x1000A000 = 0x101;
    sceGsSyncPath(0, 0);
}
