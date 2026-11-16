/* rodata: 0x00675C08-0x00675C10 */
#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

char *strcpy(char *, const char *);
void strcat(char *, char *);
void FldMenuSaveSlotListBuild(s32, s32, s32);

void McLoadSelectSlot(void) {
    char buf[16];
    McPortState *st;
    McSlotTable *obj;
    s32 done;
    u16 k;

    done = 0;
    McSetPollInterval(0x3C);
    st = McGetCurPort();
    obj = McGetSlotTable();
    mcLoadSlotCursor = mcLoadSlot;
    /* do-while(0): loop-depth weight that sets the s-register order */
    do {
        /* do-while(0): keeps the pad loads after the McIsBusy test */
        do {
            if (McIsBusy() != 0) {
                goto clamp;
            }
        } while (0);
        k = mcLoadPad->repeat;
        if (k & 0x4000) {
            goto inc;
        }
        if (k & 0x1000) {
            mcLoadSlotCursor--;
            goto clamp;
        }
        k = mcLoadPad->trigger;
        if (k & 0x40) {
            done = 1;
            McLoadSetState(0);
            mcLoadChoice = mcLoadSlotCursor;
            SndSePlay(0x7D3, 0, 0, 0x1E, 0);
            goto clamp;
        }
        if (k & 0x20) {
            sprintf(buf, mcLoadSlotNumFmt, mcLoadSlotCursor);
            strcpy(mcSaveFileEntry.name, "save");
            strcat(mcSaveFileEntry.name, buf);
            strcat(mcSaveFileEntry.name, mcLoadFileExt);
            if (obj->slots[mcLoadSlotCursor].used != 0 && obj->slots[mcLoadSlotCursor].status >= 0) {
                McLoadSetState(3);
                SndSePlay(0x7D2, 0, 0, 0x1E, 0);
            } else {
                SndSePlay(0x7D4, 0, 0, 0x1E, 0);
            }
        }
        if (st->state < 2) {
            return;
        }
        goto clamp;
    inc:
        mcLoadSlotCursor++;
    clamp:
        if (mcLoadSlotCursor < 0) {
            mcLoadSlotCursor = 0;
        }
        if (mcLoadSlotCursor >= 8) {
            mcLoadSlotCursor = 7;
        }
        if (mcLoadSlot != mcLoadSlotCursor) {
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        }
    } while (0);
    mcLoadSlot = mcLoadSlotCursor;
    /* do-while(0): the final dialog calls stay jal (unwrapped they become tail calls and the blocks move) */
    do {
        if (mcLoadStateChanged != 0) {
            FldMenuSaveSlotListBuild(0, mcLoadSlotCursor, 3);
        } else if (done) {
            FldMenuSaveSlotListBuild(2, mcLoadSlotCursor, 3);
        } else {
            FldMenuSaveSlotListBuild(1, mcLoadSlotCursor, 3);
        }
    } while (0);
}
