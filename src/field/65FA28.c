#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McLoadRead(void) {
    s32 h;
    s32 n;
    s32 i;
    s32 done;

    done = 0;
    if (mcLoadStateChanged != 0) {
        mcLoadReadReqs[0] = McReqChdir((s32)mcLoadDirPath, 0);
        h = McReqOpen(&mcSaveFileEntry, 1);
        mcLoadReadReqs[1] = h;
        mcLoadReadReqs[2] = McReqRead(h, (s32)mcSaveFileEntry.data, mcSaveFileEntry.size);
        mcLoadReadReqs[3] = McReqClose(h);
        mcLoadReadDelay = 0x1E;
    } else {
        /* do-while(0) as a breakable block: the breaks are its forward exits, laid out as a loop */
        do {
            if (--mcLoadReadDelay > 0) {
                break;
            }
            if (McIsBusy() != 0) {
                break;
            }
            McGetCurPort();
            n = McReqGetStatus(mcLoadReadReqs[3]);
            McReqGet(mcLoadReadReqs[2]);
            if (n > 0 && McSaveCheckChecksum(mcSaveDataPtr) != 0) {
                done = 1;
                McLoadSetState(0x304);
                SndSePlay(0x7D2, 0, 0, 0x1E, 0);
            } else {
                done = 1;
                McLoadSetState(0x30A);
                SndSePlay(0x7D6, 0, 0, 0x1E, 0);
            }
            for (i = 0; i < 4; i++) {
                McReqFreeId(mcLoadReadReqs[i]);
            }
        } while (0);
    }
    /* do-while(0): the final dialog calls stay jal (unwrapped they become tail calls and the blocks move) */
    do {
        if (mcLoadStateChanged != 0) {
            FldMenuMcDlgLoading(0);
        } else if (done) {
            FldMenuMcDlgLoading(2);
        } else {
            FldMenuMcDlgLoading(1);
        }
    } while (0);
}
