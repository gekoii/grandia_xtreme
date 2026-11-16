#include "common.h"
#include "gph.h"
#include "main/n07.h"

/* Swaps two globals. */
void GphFbSwapDrawZBuf(void) {
    s32 temp = gsZBufAddr;

    gsZBufAddr = gsDrawFrameAddr;
    gsDrawFrameAddr = temp;
}
