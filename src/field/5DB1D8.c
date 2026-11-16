#include "common.h"
#include "pad.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n16.h"

extern s32 dbgFldOccsNo;
extern char dbgOccsStrNumber[];
extern char dbgOccsStrCat[];
extern char dbgOccsStrEnabledOn[];
extern char dbgOccsStrEnabledOff[];
extern char dbgOccsStrDisabledOn[];
extern char dbgOccsStrDisabledOff[];

void DbgFldOccsPage(void) {
    Pad *pad;
    u16 btn;
    OccsEntry *info;

    pad = &padPort1;
    if (!(pad->held & 2)) {
        btn = pad->repeatFast;
        if ((btn & 0x8000) && dbgFldOccsNo != 0) {
            dbgFldOccsNo--;
        }
        if ((btn & 0x2000) && dbgFldOccsNo < OccsGetCount() - 1) {
            dbgFldOccsNo++;
        }
        if (pad->trigger & 0x20) {
            OccsWake(dbgFldOccsNo);
        }
    }
    info = OccsGet(dbgFldOccsNo);
    DbgPrintf(2, 4, dbgOccsStrNumber, dbgFldOccsNo, OccsGetCount());
    if (info != NULL) {
        DbgPrintf(2, 3, dbgOccsStrCat, OCC_ID_CAT(info->id));
        DbgPrintf(14, 3, dbgOccStrGrp, OCC_ID_GRP(info->id));
        DbgPrintf(24, 3, dbgOccStrIndex, OCC_ID_IDX(info->id));
        if (info->flags & 0x10) {
            DbgPrintf(2, 5, dbgOccsStrEnabledOn);
        } else {
            DbgPrintf(2, 5, dbgOccsStrEnabledOff);
        }
        if (info->flags & 0x20) {
            DbgPrintf(2, 6, dbgOccsStrDisabledOn);
        } else {
            DbgPrintf(2, 6, dbgOccsStrDisabledOff);
        }
        DbgPrintf(24, 4, dbgOccStrStatus, dbgFldOccStatusNames[info->state]);
        DbgPrintf(24, 5, dbgOccStrCount, info->timer);
        if (info->flags & 1) {
            DbgPrintf(24, 6, dbgOccStrLoopOn);
        } else {
            DbgPrintf(24, 6, dbgOccStrLoopOff);
        }
        if (info->flags & 2) {
            DbgPrintf(24, 7, dbgOccStrAutoWakeOn);
        } else {
            DbgPrintf(24, 7, dbgOccStrAutoWakeOff);
        }
        DbgPrintf(24, 8, dbgOccStrAttribute, (s16)info->flags);
    }
}
