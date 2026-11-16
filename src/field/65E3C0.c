#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "field/n26.h"


void McSaveWrite(void) {
    McSlotTable *obj;
    McPortState *st;
    s32 r;
    s32 h;
    s32 i;
    s32 done;

    done = 0;
    if (McGetCurPort()->state < 2) {
        McSaveSetState(0x303);
        SndSePlay(0x7D6, 0, 0, 0x1E, 0);
        return;
    }
    obj = McGetSlotTable();
    if (mcSaveStateChanged != 0) {
        mcSaveWriteStep = 0;
        if (obj->dirExists != 0) {
            mcSaveWriteStep = 2;
        }
        if (obj->fileMask == 7 || mcSaveFreeKB < mcSaveGameFilesNeedKB - mcSaveGameFilesKB) {
            mcSaveWriteStep = 4;
        }
        mcSaveWriteIsNew = 0;
    }
    switch (mcSaveWriteStep) {
    case 0:
        McSaveMakeDir(0);
        mcSaveWriteStep++;
        break;
    case 1:
        r = McSaveMakeDir(1);
        if (r == 0) {
            break;
        }
        if (r < 0) {
            goto fail;
        }
        obj->dirExists = 1;
        mcSaveWriteStep++;
    case 2:
        McSaveWriteIconFiles(0);
        mcSaveWriteStep++;
        break;
    case 3:
        r = McSaveWriteIconFiles(1);
        if (r == 0) {
            break;
        }
        if (r < 0) {
        fail:
            McSaveSetState(0x303);
            break;
        }
        mcSaveGameFilesKB = mcSaveGameFilesNeedKB;
        obj->fileMask = 7;
        mcDirListPtr->count = 5;
        mcSaveWriteStep++;
    case 4:
        mcSaveWriteReqs[0] = McReqChdir(mcSaveFlowDirPath, 0);
        if (mcSaveSlotRec->used != 0) {
            h = McReqOpen(&mcSaveFileEntry, 2);
            mcSaveWriteReqs[1] = h;
            mcSaveWriteIsNew = 0;
        } else {
            h = McReqOpen(&mcSaveFileEntry, 0x200);
            mcSaveWriteReqs[1] = h;
            mcSaveWriteIsNew = 1;
        }
        mcSaveWriteReqs[2] = McReqWrite(h, mcSaveFileEntry.data, mcSaveFileEntry.size);
        mcSaveWriteReqs[3] = McReqClose(h);
        sysSavePlayTime = mcSavePlayTimeNow;
        McSaveSetChecksum(mcSaveFileEntry.data, mcSaveFileEntry.size);
        mcSaveWriteStep++;
        break;
    case 5:
        /* do-while(0): keeps this case's block order (its calls are laid out first unwrapped) */
        do {
            if (McIsBusy() != 0) {
                goto out;
            }
            st = McGetCurPort();
            if (McReqGetStatus(mcSaveWriteReqs[3]) > 0) {
                McSaveSetState(0x304);
                SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                if (mcSaveWriteIsNew != 0) {
                    mcDirListPtr->count++;
                    mcSaveWriteIsNew = 0;
                }
            } else {
                McSaveSetState(0x303);
                SndSePlay(0x7D6, 0, 0, 0x1E, 0);
            }
            for (i = 0; i < 4; i++) {
                McReqFreeId(mcSaveWriteReqs[i]);
            }
            done = 1;
            mcSaveFreeKB = st->freeKB;
        } while (0);
    out:
        break;
    }
    /* do-while(0): the final dialog calls stay jal (unwrapped they become tail calls and the blocks move) */
    do {
        if (mcSaveStateChanged != 0) {
            FldMenuMcDlgSaving(0);
        } else if (done) {
            FldMenuMcDlgSaving(2);
        } else {
            FldMenuMcDlgSaving(1);
        }
    } while (0);
}
