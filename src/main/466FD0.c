#include "common.h"
#include "snd.h"

s32 WaitSema(s32);
void SignalSema(s32);

void viBufFlush(ViBuf *arg0) {
    WaitSema(arg0->sema);
    arg0->readBytes = (arg0->readBytes + 0x7FF) / 0x800 * 0x800;
    SignalSema(arg0->sema);
}
