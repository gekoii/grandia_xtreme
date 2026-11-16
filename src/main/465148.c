#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

extern s32 AddIntcHandler(s32, void *, s32);
extern s32 AddDmacHandler(s32, void *, s32);
extern s32 CreateThread(struct ThreadParam *);
extern s32 StartThread(s32, void *);
extern s32 EnableIntc(s32);
extern s32 EnableDmac(s32);
extern void videoCallback(void);

void MovListInit(void) {
    struct ThreadParam tp;
    s32 i;
    VideoDec *c;

    *(volatile u32 *)0x1000E000 |= 3;
    *(volatile u32 *)0x1000E010 = 4;
    readBufCreate(movReadBuf);
    MovDispInit(&movDisp, 0x280, 0xE0, 0, 0, 0x280, 0xE0, 0x280, 0x1C0);
    MovClearScreen(&movDisp, 0);
    sceMpegInit();
    videoDecCreate(&movVideoDec, movMpegWork, 0x13B800, movViBufData, movViBufTag, 0x80, movTimeStamps, 0x280);
    videoDecSetStream(&movVideoDec, 0, 0, videoCallback, movReadBuf);
    voBufCreate(&movVoBuf, (movVoBufData & 0x0FFFFFFF) | 0x20000000, movVoBufTag, 5);
    for (i = 0; i < movVoBuf.size; i++) {
        if (movListType == 6) {
            MovIpartSetImageTag(&movDisp, (s32)movVoBuf.tag[i]->v[0], movVoBuf.data + i * 0x118000, 0);
            MovIpartSetImageTag(&movDisp, (s32)movVoBuf.tag[i]->v[1], movVoBuf.data + i * 0x118000, 1);
        } else {
            MovListSetImageTag(&movDisp, (s32)movVoBuf.tag[i]->v[0], movVoBuf.data + i * 0x118000, 0);
            MovListSetImageTag(&movDisp, (s32)movVoBuf.tag[i]->v[1], movVoBuf.data + i * 0x118000, 1);
        }
    }
    c = &movVideoDec;
    tp.entry = defMain;
    tp.stack = movDefStack;
    tp.stackSize = 0x800;
    tp.initPriority = movMainPrio;
    tp.gpReg = threadGp;
    tp.option = 0;
    movDefThreadId = CreateThread(&tp);
    StartThread(movDefThreadId, 0);
    tp.entry = videoDecMain;
    tp.stack = movDecStack;
    tp.stackSize = 0x4000;
    tp.initPriority = movMainPrio;
    tp.gpReg = threadGp;
    tp.option = 0;
    movDecThreadId = CreateThread(&tp);
    StartThread(movDecThreadId, c);
    if (movListType == 6) {
        c->intcHandlerId = AddIntcHandler(2, MovIpartVblankHandler, 0);
    } else {
        c->intcHandlerId = AddIntcHandler(2, MovListVblankHandler, 0);
    }
    EnableIntc(2);
    movVideoDec.dmacHandlerId = AddDmacHandler(2, MovGifEndHandler, 0);
    EnableDmac(2);
}
