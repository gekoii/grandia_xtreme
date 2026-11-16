/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"

extern char _scePadEmptyStr[];
void strcpy(char *, char *);

void scePadStateIntToStr(u32 arg0, char *arg1) {
    if (arg0 < 8) {
        strcpy(arg1, _scePadStateStrTable[arg0]);
        return;
    }
    *arg1 = _scePadEmptyStr[0];
}
