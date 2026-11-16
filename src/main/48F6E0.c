/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

extern void SleepThread(void);
extern s32 sceSifGetNextRequest(s32);

void sceSifRpcLoop(s32 arg0) {
    s32 v;

    while (1) {
        while ((v = sceSifGetNextRequest(arg0)) != 0) {
            sceSifExecRequest(v);
        }
        SleepThread();
    }
}
