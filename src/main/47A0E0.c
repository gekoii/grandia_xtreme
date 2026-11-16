/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"

void strcpy(u8 *, char *);
extern u8 _scePadEmptyStr;

void scePadReqIntToStr(u32 arg0, u8 *arg1) {
    if (arg0 < 4) {
        strcpy(arg1, _scePadReqStateStrTable[arg0]);
        return;
    }
    *arg1 = _scePadEmptyStr;
}
