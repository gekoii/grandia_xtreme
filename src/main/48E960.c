/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "protos/main.h"


void sceSifExitRpc(void) {
    sceSifExitCmd();
    _sceSifRpcInitDone = 0;
}
