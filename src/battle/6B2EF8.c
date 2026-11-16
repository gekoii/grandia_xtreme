/* rodata: 0x006E15F8-0x006E1608 */
#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

s32 MgcSeLoadMagic(void) {
    char buf[256];
    s16 n = mgcId;

    if (mgcSeLoadBusy == 0) {
        if (n < 91) {
            sprintf(buf, "MUSIC/M%2.1d.SE", n);
            BtlStrSpaceToZero(buf);
            SndSeBankLoadStart(buf, 0, n + 0x1A8F, 0);
        }
        mgcSeLoadBusy = 1;
    } else if (SndSeBankLoadGetState() == -1) {
        mgcSeLoadBusy = 0;
        return 1;
    }
    return 0;
}
