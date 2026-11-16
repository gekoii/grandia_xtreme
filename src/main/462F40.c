#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

s32 SndHandleSetVolume(SndHandle *arg0, s32 arg1) {
    s32 ret;
    s32 y;
    s32 x;
    SndIndexEntry *e;
    s32 id;

    ret = -4;
    switch (arg0->type) {
    case 5:
        e = &sndIndexTable[arg0->index];
        id = e->u.s.volume;
        SndCalcVolPan(id & 0x7FFF, arg1, arg0->dist, arg0->pan, &x, &y);
        ret = SndIopCmdPush(0x230, arg0->voice, x, y);
        break;
    case 4:
        ret = SndIopCmdPush(0x234, arg0->chan, arg1, 0);
        break;
    case 6:
        break;
    }
    return ret;
}
