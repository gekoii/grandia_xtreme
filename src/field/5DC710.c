#include "common.h"
#include "pad.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n16.h"

extern s32 dbgFldMonsterTableNo;
extern char dbgMonTblStrTitle[];
extern char dbgMonTblStrTableNo[];

void DbgFldMonsterTablePage(void) {
    Pad *pad;
    FldEncGroups *tbl;
    s32 n;
    u16 btn;
    FldEncGroup *r;

    pad = &padPort1;
    tbl = &fldEncGroups;
    n = tbl->count;
    DbgPrintf(4, 5, dbgMonTblStrTitle);
    if (n != 0 && tbl->groups != NULL) {
        if (!(padPort1.held & 2)) {
            btn = pad->trigger;
            if (btn & 0x8000) {
                if (dbgFldMonsterTableNo != 0) {
                    dbgFldMonsterTableNo--;
                } else {
                    dbgFldMonsterTableNo = n - 1;
                }
            }
            if (btn & 0x2000) {
                dbgFldMonsterTableNo++;
            }
        }
        if (dbgFldMonsterTableNo >= n) {
            dbgFldMonsterTableNo = 0;
        }
        DbgDrawCursor(3, 6);
        r = &fldEncGroups.groups[dbgFldMonsterTableNo];
        DbgPrintf(4, 6, dbgMonTblStrTableNo, dbgFldMonsterTableNo, n);
        DbgPrintf(4, 7, dbgChrStrGroupMax, r->count);
        DbgPrintf(4, 8, dbgChrStrGroupPoint, r->cost);
        DbgPrintf(4, 9, dbgChrStrGroupInfHeader);
        DbgPrintf(4, 10, dbgChrStrGroupNo, r->slots[0].defNo, r->slots[1].defNo, r->slots[2].defNo, r->slots[3].defNo);
        DbgPrintf(4, 11, dbgChrStrGroupMaxRow, r->slots[0].count, r->slots[1].count, r->slots[2].count, r->slots[3].count,
                      r->slots[0].count + r->slots[1].count + r->slots[2].count + r->slots[3].count);
        DbgPrintf(4, 12, dbgChrStrGroupLevel, r->slots[0].level, r->slots[1].level, r->slots[2].level, r->slots[3].level,
                      r->slots[0].level + r->slots[1].level + r->slots[2].level + r->slots[3].level);
    }
}
