#include "common.h"
#include "pad.h"
#include "save.h"
#include "protos/field.h"
#include "field/n16.h"

extern s32 dbgFldGameDebugPage;
extern char dbgGameDebugStrTitle[];
extern char dbgGameDebugStrMainFlag[];
extern char dbgGameDebugStrGlobalFlag[];
extern char dbgGameDebugStrSubGameFlag[];
extern char dbgGameDebugStrXvsFlag[];
extern char dbgGameDebugStrSystemFlag[];
extern char dbgGameDebugStrLocalFlag[];
extern char dbgGameDebugStrTreasureFlag[];
extern char dbgGameDebugStrMonsterFlag[];
extern char dbgGameDebugStrDungeonFlag[];
extern char dbgGameDebugStrGameFlag[];
extern char dbgGameDebugStrAreaMapFlag[];
extern char dbgGameDebugStrReaderFlag[];
extern char dbgGameDebugStrTalkCounter[];
extern char dbgGameDebugStrPointTalkCounter[];
extern char dbgGameDebugStrLocalCounter[];
extern char dbgGameDebugStrGlobalCounter[];
extern char dbgGameDebugStrMealCounter[];
extern char dbgGameDebugStrSubTime1Counter[];
extern char dbgGameDebugStrSubTime2Counter[];
extern char dbgGameDebugStrXvsCounter[];
extern char dbgGameDebugStrSubGameCounter[];
extern char dbgGameDebugStrFaceList[];
extern char dbgGameDebugStrFaceTable[];
extern char dbgGameDebugStrPartyMember[];
extern u8 evtTalkCounters[];
extern u8 evtPointTalkCounters[];
extern u8 evtLocalCounters[];
extern u8 evtGlobalCounters[];
extern u8 evtMealCounters[];
extern u8 evtSubTime1Counters[];
extern u8 evtSubTime2Counters[];
extern u8 evtXvsCounters[];
extern u8 evtSubGameCounters[];

void DbgFldGameDebugMenu(void) {
    Pad *pad = &padPort1;

    DbgPrintf(4, 3, dbgGameDebugStrTitle, dbgFldPageColor5Str, dbgFldGameDebugPage + 1, dbgFldPageColor0Str);

    if (pad->trigger & 4) {
        if (dbgFldGameDebugPage == 0) {
            dbgFldGameDebugPage = 25;
        } else {
            dbgFldGameDebugPage--;
        }
    }
    if (pad->trigger & 8) {
        if (dbgFldGameDebugPage < 25) {
            dbgFldGameDebugPage++;
        } else {
            dbgFldGameDebugPage = 0;
        }
    }
    switch (dbgFldGameDebugPage) {
    case 0: DbgPrintf(4, 4, dbgGameDebugStrMainFlag); break;
    case 1: DbgPrintf(4, 4, dbgGameDebugStrGlobalFlag, 0x400); break;
    case 2: DbgPrintf(4, 4, dbgGameDebugStrSubGameFlag, 0x200); break;
    case 3: DbgPrintf(4, 4, dbgGameDebugStrXvsFlag, 0x200); break;
    case 4: DbgPrintf(4, 4, dbgGameDebugStrSystemFlag, 0x100); break;
    case 5: DbgPrintf(4, 4, dbgGameDebugStrLocalFlag, 0x80); break;
    case 6: DbgPrintf(4, 4, dbgGameDebugStrTreasureFlag, 0x80); break;
    case 7: DbgPrintf(4, 4, dbgGameDebugStrMonsterFlag, 0x100); break;
    case 8: DbgPrintf(4, 4, dbgGameDebugStrDungeonFlag, 0x40); break;
    case 9: DbgPrintf(4, 4, dbgGameDebugStrGameFlag, 0x80); break;
    case 10: DbgPrintf(4, 4, dbgGameDebugStrAreaMapFlag, 0x20); break;
    case 11: DbgPrintf(4, 4, dbgGameDebugStrReaderFlag, 0x10); break;
    case 12: DbgPrintf(4, 4, dbgGameDebugStrTalkCounter, 0x20); break;
    case 13: DbgPrintf(4, 4, dbgGameDebugStrPointTalkCounter, 8); break;
    case 14: DbgPrintf(4, 4, dbgGameDebugStrLocalCounter, 4); break;
    case 15: DbgPrintf(4, 4, dbgGameDebugStrGlobalCounter, 0x30); break;
    case 16: DbgPrintf(4, 4, dbgGameDebugStrMealCounter, 8); break;
    case 17: DbgPrintf(4, 4, dbgGameDebugStrSubTime1Counter, 0x28); break;
    case 18: DbgPrintf(4, 4, dbgGameDebugStrSubTime2Counter, 0x28); break;
    case 19: DbgPrintf(4, 4, dbgGameDebugStrXvsCounter, 0x30); break;
    case 20: DbgPrintf(4, 4, dbgGameDebugStrSubGameCounter, 0x30); break;
    case 21: DbgPrintf(4, 4, dbgGameDebugStrFaceList); break;
    case 22: DbgPrintf(4, 4, dbgGameDebugStrFaceTable); break;
    case 23: DbgPrintf(4, 4, dbgGameDebugStrPartyMember); break;
    }
    switch (dbgFldGameDebugPage) {
    case 0: DbgFldMainFlagPage(); break;
    case 1: DbgFldFlagEditPage(evtGlobalFlags, 0x80); break;
    case 2: DbgFldFlagEditPage(evtSubGameFlags, 0x40); break;
    case 3: DbgFldFlagEditPage(evtXvsFlags, 0x40); break;
    case 4: DbgFldFlagEditPage(evtSystemFlags, 0x20); break;
    case 5: DbgFldFlagEditPage(evtLocalFlags, 0x10); break;
    case 6: DbgFldFlagEditPage(evtTreasureFlags, 0x10); break;
    case 7: DbgFldFlagEditPage(evtMonsterFlags, 0x20); break;
    case 8: DbgFldFlagEditPage(evtDungeonFlags, 8); break;
    case 9: DbgFldFlagEditPage(evtGameFlags, 0x10); break;
    case 10: DbgFldFlagEditPage(evtAreaMapFlags, 4); break;
    case 11: DbgFldFlagEditPage(evtReaderFlags, 2); break;
    case 12: DbgFldCounterEditPage(evtTalkCounters, 0x20, 0x25); break;
    case 13: DbgFldCounterEditPage(evtPointTalkCounters, 8, 0x45); break;
    case 14: DbgFldCounterEditPage(evtLocalCounters, 4, 0x4D); break;
    case 15: DbgFldCounterEditPage(evtGlobalCounters, 0x30, 0x51); break;
    case 16: DbgFldCounterEditPage(evtMealCounters, 8, 0x81); break;
    case 17: DbgFldCounterEditPage(evtSubTime1Counters, 0x28, 0x89); break;
    case 18: DbgFldCounterEditPage(evtSubTime2Counters, 0x28, 0xB1); break;
    case 19: DbgFldCounterEditPage(evtXvsCounters, 0x30, 0xD9); break;
    case 20: DbgFldCounterEditPage(evtSubGameCounters, 0x30, 0x109); break;
    case 21: DbgFldFaceListPage(); break;
    case 22: DbgFldFaceTablePage(); break;
    case 23: DbgFldPartyMemberPage(); break;
    case 24: DbgFldXvsInfoPage(); break;
    case 25: DbgFldXvsCharaPage(); break;
    }
}
