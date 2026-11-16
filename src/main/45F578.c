#include "common.h"
#include "protos/main.h"
#include "main/n10.h"

typedef struct { s32 unk0; s32 unk4; s32 unk8; } Obj;
extern s32 sndIopStatusAddr;
extern Obj *sndIopHeader;

s32 SndIopStatusRecv(void) {
    s32 r;

    r = sceSifGetOtherData(&sndIopStatusRecvData, sndIopStatusAddr, 0x01FF0100, sndIopHeader->unk8, 1);
    if (r == 0) {
        sndIopStatusRecvBusy = 1;
    } else {
        sndIopStatusRecvBusy = 0;
    }
    return r;
}
