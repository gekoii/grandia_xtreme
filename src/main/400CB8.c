#include "common.h"
#include "sys.h"
#include "protos/main.h"




void SysProgRequest(u8 arg0, s32 arg1) {
    sysState.requested[arg0] = arg1;
    ((void (*)(u8))SndSeStopAll)(arg0); /* passes arg0 to the no-parameter function (see 441D78.c) */
    if (arg1 == 3) {
        BtlHandoff *s = &btlHandoff;
        GphShatterStart(s->unk54);
        if (s->unk54 != 2) {
            SndSePlay3D(0x7DC, 3, 0, NULL);
        }
    } else {
        BtlHandoff *t = &btlHandoff;
        if (t->result == 3) {
            t->softResetReq = 1;
        } else {
            t->result++;
        }
    }
}
