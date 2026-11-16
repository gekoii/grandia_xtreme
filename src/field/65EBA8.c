#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McSaveDoneMsg(void) {
    McSlotTable *obj;
    s32 done;

    done = 0;
    if (mcSaveStateChanged != 0) {
        mcLastSlot = mcSaveSlot;
        mcLastPort = mcSavePort;
    } else if (McIsBusy() == 0) {
        if (mcSavePad->trigger != 0) {
            obj = McGetSlotTable();
            obj->slots[mcSaveSlot] = mcSaveSlotHeader;
            obj->slots[mcSaveSlot].status = 0;
            obj->slots[mcSaveSlot].used = 1;
            McSaveSetState(2);
            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
            done = 1;
        }
    }
    /* do-while(0): the final dialog calls stay jal (unwrapped they become tail calls and the blocks move) */
    do {
        if (mcSaveStateChanged != 0) {
            FldMenuMcDlgSaveDone(0);
        } else if (done) {
            FldMenuMcDlgSaveDone(2);
        } else {
            FldMenuMcDlgSaveDone(1);
        }
    } while (0);
}
