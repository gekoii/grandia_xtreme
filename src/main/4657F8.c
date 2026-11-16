#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

void RemoveIntcHandler(s32, s32);
void RemoveDmacHandler(s32, s32);
void DeleteThread(s32);
void TerminateThread(s32);
void DisableIntc(s32);
void DisableDmac(s32);

void MovListTermPart(void) {
    VideoDec *p;
    voBufDelete(&movVoBuf);
    TerminateThread(movDecThreadId);
    DeleteThread(movDecThreadId);
    TerminateThread(movDefThreadId);
    DeleteThread(movDefThreadId);
    DisableDmac(2);
    p = &movVideoDec;
    RemoveDmacHandler(2, p->dmacHandlerId);
    DisableIntc(2);
    RemoveIntcHandler(2, p->intcHandlerId);
    videoDecDelete(p);
}
