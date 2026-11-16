#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

extern s32 AddIntcHandler(s32, void *, s32);
extern s32 AddDmacHandler(s32, void *, s32);
extern s32 CreateThread(struct ThreadParam *);
extern s32 StartThread(s32, void *);
extern s32 EnableIntc(s32);
extern s32 EnableDmac(s32);
extern u8 movAudioBuf[];
extern void videoCallback(void);

void MovInit(void) {
    struct ThreadParam tp;
    s32 i;
    s32 k1 = 0;
    s32 k2 = 0;
    s32 k4 = 0;
    s32 k5 = 2;
    s32 k7 = 2;
    s32 k9 = 0;
    s32 k10 = 2;

    i = 0;
    *(volatile s32 *)0x1000E000 |= 3;
    *(volatile s32 *)0x1000E010 = 4;
    readBufCreate(movReadBuf);
    MovDispInit(&movDisp, 0x280, 0xE0, 0, 0, 0x280, 0xE0, 0x280, 0x1C0);
    sceMpegInit();
    videoDecCreate(&movVideoDec, movMpegWork, 0x13B800, movViBufData, movViBufTag, 0x80, movTimeStamps, 0x280);
    audioDecCreate(&movAudioDec, movAudioBuf, 0x2400, 0x1000);
    videoDecSetStream(&movVideoDec, 0, 0, videoCallback, movReadBuf);
    voBufCreate(&movVoBuf, (movVoBufData & 0x0FFFFFFF) | 0x20000000, movVoBufTag, 5);
    for (; i < movVoBuf.size; i++) {
        MovSetImageTag(&movDisp, (s32)movVoBuf.tag[i]->v[0], movVoBuf.data + i * 0x118000, 0);
        MovSetImageTag(&movDisp, (s32)movVoBuf.tag[i]->v[1], movVoBuf.data + i * 0x118000, 1);
    }
    tp.entry = defMain;
    tp.stack = movDefStack;
    tp.stackSize = 0x800;
    tp.initPriority = movMainPrio;
    tp.gpReg = threadGp;
    tp.option = k2;
    movDefThreadId = CreateThread(&tp);
    StartThread(movDefThreadId, (void *)k1);
    tp.entry = videoDecMain;
    tp.stack = movDecStack;
    tp.stackSize = 0x4000;
    tp.initPriority = movMainPrio;
    tp.gpReg = threadGp;
    tp.option = k4;
    movDecThreadId = CreateThread(&tp);
    StartThread(movDecThreadId, &movVideoDec);
    movVideoDec.intcHandlerId = AddIntcHandler(k5, MovVblankHandler, 0);
    EnableIntc(k7);
    movVideoDec.dmacHandlerId = AddDmacHandler(2, MovGifEndHandler, k9);
    EnableDmac(k10);
}
