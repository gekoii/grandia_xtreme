/* rodata: 0x004A47C0-0x004A4820 */
#include "common.h"
#include "pad.h"
#include "snd.h"
#include "protos/main.h"

typedef struct {
    char n[5][5];
} Names;

extern Names dbgSndSeFlagNames;
extern s32 dbgSndSeNo;
extern s32 dbgSndSeLine;
extern s32 dbgSndSeCursor;
extern s32 dbgSndSeHpan;

void DbgSoundSePage(void) {
    Names names = dbgSndSeFlagNames;
    s32 line;
    s32 i;
    s32 j;
    SndSeSlotShifted *s;

    dbgPad = &padPort1;
    if (!(padPort1.held & 2)) {
        dbgSndSeLine = DbgMenuLineSelect(dbgSndSeLine, 2);
        DbgMenuCursorDraw(2, dbgSndSeLine + 4);
        switch (dbgSndSeLine) {
        case 0:
            dbgSndSeCursor = 0;
            if (!(dbgPad->held & 1)) {
                if (dbgPad->repeatFast & 0x8000) {
                    dbgSndSeNo--;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    dbgSndSeNo++;
                }
            } else {
                if (dbgPad->repeatFast & 0x8000) {
                    dbgSndSeNo -= 10;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    dbgSndSeNo += 10;
                }
            }
            if (dbgSndSeNo < 0) {
                dbgSndSeNo = 0;
            }
            break;
        case 1:
            dbgSndSeCursor = dbgSndSeLine;
            if (!(dbgPad->held & 1)) {
                if (dbgPad->repeatFast & 0x8000) {
                    dbgSndSeHpan--;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    dbgSndSeHpan++;
                }
            } else {
                if (dbgPad->repeatFast & 0x8000) {
                    dbgSndSeHpan -= 10;
                }
                if (dbgPad->repeatFast & 0x2000) {
                    dbgSndSeHpan += 10;
                }
            }
            break;
        }
        if (dbgPad->repeatFast & 0x20) {
            SndSePlay(dbgSndSeNo, 0, 0, 0x40, (u8)dbgSndSeHpan);
        }
    }
    line = 0;
    DbgPrintf(2, 4, "SE no %d", dbgSndSeNo);
    DbgPrintf(2, 5, "HPAN  %d", dbgSndSeHpan);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 64; j++) {
            s = &sndSeSlots[i].u.shifted[j];
            if (s->state != 0) {
                DbgPrintf(2, line + 7, "%2d PRIO %d FLG %s IDX %04d OBJ %d WAIT %d", line, i, names.n[s->state], s->index, s->handle, s->wait);
                line++;
            }
        }
    }
    DbgPrintf(2, 6, "PLAY MAX %d", line);
}
