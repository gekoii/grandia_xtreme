#include "common.h"
#include "save.h"


s32 EvtFlagGetByType(s16 type, s16 id) {
    s32 idx = id >> 3;
    s32 bit = (u16)id & 7;
    u8 *p;

    switch (type) {
    case 0:
        p = idx + evtGlobalFlags;
        break;
    case 1:
        p = idx + evtSystemFlags;
        break;
    case 2:
        p = idx + evtLocalFlags;
        break;
    case 3:
        p = idx + evtTreasureFlags;
        break;
    case 4:
        p = idx + evtMonsterFlags;
        break;
    case 5:
        p = idx + evtDungeonFlags;
        break;
    case 6:
        p = idx + evtGameFlags;
        break;
    default:
        return 0;
    }
    return (*p >> bit) & 1;
}
