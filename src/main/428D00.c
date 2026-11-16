#include "common.h"
#include "protos/main.h"
#include "pad.h"
#include "sys.h"
#include "main/n02.h"
#include "main/n06.h"

extern s32 dbgMemPartition;
extern s32 dbgMemStrMemory;
extern s32 dbgMemStrUseedMFreeMTotalM;
extern s32 dbgMemStrLibraryPartation;
extern s32 dbgMemStrFieldPartation;
extern s32 dbgMemStrBattlePartation;
extern s32 dbgMemStrBattleMagicPartation;
extern s32 dbgMemStrDataPartation;
extern s32 dbgMemStrLocalPartation;
extern s32 dbgMemStrFacePartation;
extern s32 dbgMemStrHandle;
extern s32 dbgMemStrStat;
extern s32 dbgMemStrAdrsStart;
extern s32 dbgMemStrEnd;
extern s32 dbgMemStrSizeUsedM;
extern s32 dbgMemStrFreeM;
extern s32 dbgMemStrTotalM;

#define MB(x) ((x) / 0x100000)
#define KB(x) ((x) % 0x100000 / 0x400)

void DbgMemoryPage(void) {
    s32 id;
    s32 i;
    s32 total;
    s32 used;
    s32 u;
    s32 f;
    MemHeap *info;

    id = 0;
    dbgPad = &padPort1;
    if (!(padPort1.held & 2)) {
        if ((padPort1.repeatFast & 0x8000) && dbgMemPartition != 0) {
            dbgMemPartition--;
        }
        if ((dbgPad->repeatFast & 0x2000) && dbgMemPartition < 6) {
            dbgMemPartition++;
        }
    }
    total = 0;
    used = 0;
    for (i = 0; i < 7; i++) {
        switch (i) {
        case 0:
            id = sysState.heap30;
            break;
        case 1:
            id = sysState.heap20;
            break;
        case 2:
            id = sysState.battleHeap;
            break;
        case 3:
            id = sysState.battleMagicHeap;
            break;
        case 4:
            id = sysState.dataHeap;
            break;
        case 5:
            id = sysState.heap38;
            break;
        case 6:
            id = sysState.faceHeap;
            break;
        }
        info = MemHeapFind(id);
        total += info->size;
        if (info->head != 0) {
            MemHeapSelect(id);
            used += info->size - MemGetFreeTotal();
        }
    }
    DbgPrintf(2, 3, &dbgMemStrMemory);
    DbgPrintf(2, 4, &dbgMemStrUseedMFreeMTotalM, MB(used), KB(used), MB(total - used), KB(total - used), MB(total), KB(total));
    switch (dbgMemPartition) {
    case 0:
    default:
        id = sysState.heap30;
        DbgPrintf(3, 5, &dbgMemStrLibraryPartation);
        break;
    case 1:
        id = sysState.heap20;
        DbgPrintf(3, 5, &dbgMemStrFieldPartation);
        break;
    case 2:
        id = sysState.battleHeap;
        DbgPrintf(3, 5, &dbgMemStrBattlePartation);
        break;
    case 3:
        id = sysState.battleMagicHeap;
        DbgPrintf(3, 5, &dbgMemStrBattleMagicPartation);
        break;
    case 4:
        id = sysState.dataHeap;
        DbgPrintf(3, 5, &dbgMemStrDataPartation);
        break;
    case 5:
        id = sysState.heap38;
        DbgPrintf(3, 5, &dbgMemStrLocalPartation);
        break;
    case 6:
        id = sysState.faceHeap;
        DbgPrintf(3, 5, &dbgMemStrFacePartation);
        break;
    }
    DbgPrintf(3, 6, &dbgMemStrHandle, id);
    info = MemHeapFind(id);
    if (info != NULL) {
        DbgPrintf(3, 7, &dbgMemStrStat, (u16)info->dirty); /* (u16)/(u32): the page prints them unsigned */
        DbgPrintf(3, 8, &dbgMemStrAdrsStart, info->base);
        DbgPrintf(3, 9, &dbgMemStrEnd, info->end);
        info = MemHeapFind(id);
        if (info->head == 0) {
            u = 0;
            f = info->size;
        } else {
            MemHeapSelect(id);
            u = info->size - MemGetFreeTotal();
            f = MemGetFreeTotal();
        }
        DbgPrintf(3, 10, &dbgMemStrSizeUsedM, u, MB(u), KB(u));
        DbgPrintf(3, 11, &dbgMemStrFreeM, f, MB(f), KB(f));
        DbgPrintf(3, 12, &dbgMemStrTotalM, (u32)info->size, MB((u32)info->size), KB((u32)info->size));
        if (!(dbgPad->held & 2) && (dbgPad->repeatFast & 0x20)) {
            dbgMemDumpBase = info->base;
            sysState.dbgCursor = 0;
            sysState.dbgMenuFlags = (sysState.dbgMenuFlags ^ 0x70000) | 0x80000;
            sysState.dbgPage = 0;
        }
    }
}
