/* rodata: 0x00675B00-0x00675B08 */
#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

char *strcpy(char *, const char *);
char *strcat(char *, char *);
void FldMenuSaveSlotListBuild(s32, s32, s32);

void McSaveSelectSlot(void) {
    char buf[16];
    s32 done;
    McPortState *c;
    McSlotTable *o;
    s32 need;
    s32 ok;
    McSlot *r;
    s32 blk;

    McSetPollInterval(0x3C);
    c = McGetCurPort();
    done = 0;
    o = McGetSlotTable();
    mcSaveFreeKB = c->freeKB;
    need = ((mcSaveFileEntry.size + 0x3FF) >> 10) + 1;
    if (o->dirExists != 0) {
        ok = mcSaveFreeKB >= need + mcSaveGameFilesNeedKB - mcSaveGameFilesKB - (mcDirListPtr->count & 1);
    } else {
        ok = 1;
    }
    mcSaveSlotCursor = mcSaveSlot;
    /* do-while(0) as a breakable block: the breaks are its forward exits, laid out as a loop */
    do {
        if (McIsBusy() != 0) {
            break;
        }
        if (!(mcSavePad->repeat & 0x4000)) {
            if (mcSavePad->repeat & 0x1000) {
                mcSaveSlotCursor--;
                goto clamp;
            }
            if (mcSavePad->trigger & 0x40) {
                done = 1;
                McSaveSetState(0);
                mcSaveChoice = mcSaveSlotCursor;
                SndSePlay(0x7D3, 0, 0, 0x1E, 0);
                break;
            }
            if (mcSavePad->trigger & 0x20) {
                sprintf(buf, mcSaveSlotNumFmt, mcSaveSlotCursor);
                strcpy(mcSaveFileEntry.name, "save");
                strcat(mcSaveFileEntry.name, buf);
                strcat(mcSaveFileEntry.name, mcSaveFileExt);
                r = &o->slots[mcSaveSlotCursor];
                mcSaveSlotRec = r;
                if (r->used != 0) {
                    if (r->status == 0) {
                        McSaveSetState(0x308);
                        SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                    } else if (blk = ((u32)(r->size + 0x3FF) >> 10) + 1, mcSaveFreeKB >= need - blk + mcSaveGameFilesNeedKB - mcSaveGameFilesKB) {
                        McSaveSetState(0x308);
                        SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                    } else {
                        SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                    }
                } else if (ok != 0) {
                    McSaveSetState(3);
                    SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                } else {
                    SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                }
            }
            if (c->state < 2) {
                return;
            }
            break;
        }
        mcSaveSlotCursor++;
    } while (0);
clamp:
    if (mcSaveSlotCursor < 0) {
        mcSaveSlotCursor = 0;
    }
    if (mcSaveSlotCursor >= 8) {
        mcSaveSlotCursor = 7;
    }
    if (mcSaveSlot != mcSaveSlotCursor) {
        SndSePlay(0x7D1, 0, 0, 0x1E, 0);
    }
    mcSaveSlot = mcSaveSlotCursor;
    mcSaveSlotListMode = ok * 2;
    /* do-while(0): the final dialog calls stay jal (unwrapped they become tail calls and the blocks move) */
    do {
        if (mcSaveStateChanged != 0) {
            FldMenuSaveSlotListBuild(0, mcSaveSlotCursor, mcSaveSlotListMode);
        } else if (done) {
            FldMenuSaveSlotListBuild(2, mcSaveSlotCursor, mcSaveSlotListMode);
        } else {
            FldMenuSaveSlotListBuild(1, mcSaveSlotCursor, mcSaveSlotListMode);
        }
    } while (0);
}
