/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x00671600-0x006717C8 */
#include "common.h"
#include "pad.h"
#include "sys.h"
#include "field/state.h"
#include "protos/field.h"
#include "field/n16.h"

typedef struct {
    u8 pad0[8];
    s32 top;
    s32 use;
    s32 end;
} Inst;

extern Inst fldInstancePool;
extern s32 dbgFldSystemLine;
extern s32 dbgMovieNo;
extern s32 dbgWindowStart;

void DbgFldSystemPage(void) {
    Pad *pad;
    Inst *inst;

    pad = &padPort1;
    DbgDrawCursor(3, dbgFldSystemLine + 5);
    if (!(pad->held & 2)) {
        if ((pad->repeatFast & 0x1000) && dbgFldSystemLine != 0) {
            dbgFldSystemLine--;
        }
        if ((pad->repeatFast & 0x4000) && dbgFldSystemLine < 9) {
            dbgFldSystemLine++;
        }
        if (pad->repeatFast & 0x8000) {
            switch (dbgFldSystemLine) {
            case 0:
                if (fldState.systemSpeed >= 2) {
                    fldState.systemSpeed--;
                }
                break;
            case 1:
                dbgWindowStart--;
                break;
            case 2:
                fldState.motionSmooth -= 1.0f;
                break;
            case 3:
                fldState.talkRotSpeed -= 0.05f;
                if (fldState.talkRotSpeed < 0.05f) {
                    fldState.talkRotSpeed = 0.05f;
                }
                break;
            case 4:
                fldState.kamaeSpeed -= 0.01f;
                break;
            case 5:
                fldState.runSpeed -= 0.01f;
                break;
            case 6:
                fldState.encountRange -= 0.1f;
                break;
            case 7:
                dbgMovieNo--;
                break;
            case 8:
                EvtAreaSetUse(0);
                break;
            case 9:
                dbgMusicSeek = 0;
                SndBgmSetSeek(dbgMusicSeek);
                break;
            }
        }
        if (pad->repeatFast & 0x2000) {
            switch (dbgFldSystemLine) {
            case 0:
                fldState.systemSpeed++;
                break;
            case 1:
                dbgWindowStart++;
                break;
            case 2:
                fldState.motionSmooth += 1.0f;
                break;
            case 3:
                fldState.talkRotSpeed += 0.1f;
                break;
            case 4:
                fldState.kamaeSpeed += 0.01f;
                break;
            case 5:
                fldState.runSpeed += 0.01f;
                break;
            case 6:
                fldState.encountRange += 0.1f;
                break;
            case 7:
                dbgMovieNo++;
                break;
            case 8:
                EvtAreaSetUse(1);
                break;
            case 9:
                dbgMusicSeek = 1;
                SndBgmSetSeek(dbgMusicSeek);
                break;
            }
        }
        if (pad->repeatFast & 0x20) {
            if (dbgFldSystemLine == 7) {
                sysState.progChangeReq = 1;
                sysState.movieNo = dbgMovieNo;
                sysState.nextMapNo = MapGetNo();
            }
            if (dbgFldSystemLine == 1 && ChrPlayerIsBusy() == 0) {
                FldMenuOpen(dbgWindowStart);
            }
        }
    }
    DbgPrintf(4, 5, "SYSTEM SPEED x %d", fldState.systemSpeed);
    DbgPrintf(4, 6, "WINDOW START     %d", dbgWindowStart);
    DbgPrintf(4, 7, "MOTION SMOOTH  %.3f", fldState.motionSmooth);
    DbgPrintf(4, 8, "TALK ROT SPEED %.3f", fldState.talkRotSpeed);
    DbgPrintf(4, 9, "KAMAE SPEED    %.3f", fldState.kamaeSpeed);
    DbgPrintf(4, 10, "RUN SPEED      %.3f", fldState.runSpeed);
    DbgPrintf(4, 11, "ENCOUNT RANGE  %.3f", fldState.encountRange);
    DbgPrintf(4, 12, "MOVIE NO       %d", dbgMovieNo);
    DbgPrintf(4, 13, "HTA USE        %s", dbgFldOffOnStr[EvtAreaGetUse()]);
    DbgPrintf(4, 14, "MUSIC SEEK    %s", dbgFldOffOnStr[dbgMusicSeek]);
    inst = &fldInstancePool;
    DbgPrintf(4, 15, "INSTANCE TOP   %08x", inst->top);
    DbgPrintf(4, 16, "INSTANCE END   %08x", inst->end);
    DbgPrintf(4, 17, "INSTANCE SIZE  %08x", inst->end - inst->top);
    DbgPrintf(4, 18, "INSTANCE USE   %08x", inst->use - inst->top);
    DbgPrintf(4, 19, "INSTANCE FREE  %08x", inst->end - inst->use);
    FldMemDebugPrint();
}
