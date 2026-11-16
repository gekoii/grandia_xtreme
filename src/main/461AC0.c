#include "common.h"
#include "protos/main.h"
#include "main/n10.h"


s32 SndControl(s32 arg0, s32 *arg1) {
    SndHandle *p;

    switch (arg0) {
    case 1:
        p = SndFindHandle(arg1[0]);
        if (p->state == 0) {
            return 0;
        }
        p->unk38 = arg1[1];
        break;
    case 0x10:
        if (SndIopCmdPush(0x260, arg1[0], arg1[1], arg1[2]) == 0) {
            return -11;
        }
        break;
    case 0x100:
        if (SndIopCmdPush(0x250, arg1[0], 0, 0) == 0) {
            return -11;
        }
        break;
    }
    return 0;
}
