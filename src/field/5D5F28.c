/* rodata: 0x006715C0-0x00671600 */
#include "common.h"
#include "pad.h"
#include "gph.h"
#include "protos/field.h"
#include "field/n16.h"

extern s32 dbgFldModelLine;
extern s32 dbgFldModelTop;

void DbgFldModelPage(void) {
    s32 cnt;
    s32 i;
    s32 idx;
    GphModel *obj;
    Pad *pad;

    cnt = MapModelGetCount();
    pad = &padPort1;
    if (!(pad->held & 2)) {
        if (pad->repeatFast & 0x1000) {
            if (dbgFldModelTop != 0) {
                if (dbgFldModelLine >= 7) {
                    dbgFldModelLine--;
                } else {
                    dbgFldModelTop--;
                }
            } else if (dbgFldModelLine != 0) {
                dbgFldModelLine--;
            }
        }
        if (pad->repeatFast & 0x4000) {
            if (dbgFldModelLine < 6) {
                dbgFldModelLine++;
            } else if (dbgFldModelTop + 13 < cnt) {
                dbgFldModelTop++;
            } else if (dbgFldModelTop + dbgFldModelLine < cnt - 1) {
                dbgFldModelLine++;
            }
        }
        if (pad->repeatFast & 0x8000) {
            MapModelSetVisible(dbgFldModelTop + dbgFldModelLine, 0);
        }
        if (pad->repeatFast & 0x2000) {
            MapModelSetVisible(dbgFldModelTop + dbgFldModelLine, 1);
        }
    }
    i = 0;
    DbgDrawCursor(3, dbgFldModelLine + 5);
    DbgPrintf(4, 4, "GROUND MODEL MAX : %d", MapModelGetCount());
    do {
        obj = MapModelGetObj(dbgFldModelTop + i);
        if (obj == NULL) {
            break;
        }
        idx = dbgFldModelTop + i;
        DbgPrintf(4, i + 5, "OBJ %3d : PARTS %03d ID : %3d DISP %s", idx, obj->partCount, obj->objId,
                      dbgFldOffOnStr[MapModelIsVisible(idx)]);
        i++;
        if (cnt == i) {
            break;
        }
    } while (i < 13);
}
