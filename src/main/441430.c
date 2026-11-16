#include "common.h"
#include "protos/main.h"
#include "main/n06.h"

s32 SndStrmGetReady(s32, s32);

s32 SndSeBankLoadUpdate(void) {
    switch (sndSeWork.loadState) {
    case 0:
        sndSeWork.sectors = FileGetSizeAligned(sndSeWork.name) >> 11;
        sndSeWork.fd = SndStrmOpen(sndSeWork.name);
        if (sndSeWork.fd >= 0) {
            sndSeWork.loadState++;
        }
        break;
    case 1:
        sndSeWork.ret = SndStrmReadStart(sndSeWork.fd, sndSeWork.sectors);
        if (sndSeWork.ret >= 0) {
            sndSeWork.loadState++;
        }
        break;
    case 2:
        sndSeWork.ret = SndStrmGetReady(sndSeWork.fd, 0);
        if (sndSeWork.ret == sndSeWork.sectors) {
            sndSeWork.loadState++;
        }
        break;
    case 3:
        sndSeWork.prepare.handle = sndSeWork.bankHandle;
        sndSeWork.prepare.index = sndSeWork.bankIndex;
        sndSeWork.prepare.unk8 = sndSeWork.fd;
        sndSeWork.prepare.unkC = sndSeWork.sectors;
        sndSeWork.ret = SndPrepare(&sndSeWork.prepare);
        if (sndSeWork.ret < 0) {
            SndStrmClose(sndSeWork.fd);
            sndSeWork.loadState = -1;
        } else {
            sndSeWork.loadState++;
        }
        break;
    case 4:
        if (SndIsBankLoaded(sndSeWork.ret) == 0) {
            sndSeWork.loadState++;
            SndStrmClose(sndSeWork.fd);
        }
        break;
    case 5:
        sndSeWork.loadState = -1;
        break;
    }
    return sndSeWork.loadState;
}
