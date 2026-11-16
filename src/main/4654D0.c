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

void MovListReinit(s32 arg0) {
    struct ThreadParam tp;

    *(volatile s32 *)0x1000E000 |= 3;
    *(volatile s32 *)0x1000E010 = 4;
    readBufCreate(arg0);
    sceMpegInit();
    videoDecCreate(&movVideoDec, movMpegWork, 0x13B800, movViBufData, movViBufTag, 0x80, movTimeStamps, 0x280);
    videoDecSetStream(&movVideoDec, 0, 0, videoCallback, arg0);
    voBufCreate(&movVoBuf, (movVoBufData & 0x0FFFFFFF) | 0x20000000, movVoBufTag, 5);
    tp.entry = defMain;
    tp.stack = movDefStack;
    tp.stackSize = 0x800;
    tp.gpReg = threadGp;
    tp.initPriority = movMainPrio;
    tp.option = 0;
    movDefThreadId = CreateThread(&tp);
    StartThread(movDefThreadId, NULL);
    tp.entry = videoDecMain;
    tp.stack = movDecStack;
    tp.stackSize = 0x4000;
    tp.gpReg = threadGp;
    tp.initPriority = movMainPrio;
    tp.option = 0;
    movDecThreadId = CreateThread(&tp);
    StartThread(movDecThreadId, &movVideoDec);
    movVideoDec.intcHandlerId = AddIntcHandler(2, MovListVblankHandler, 0);
    EnableIntc(2);
    movVideoDec.dmacHandlerId = AddDmacHandler(2, MovGifEndHandler, 0);
    EnableDmac(2);
}
