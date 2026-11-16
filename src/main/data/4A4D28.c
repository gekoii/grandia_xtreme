/* main executable .rodata 0x004A4D28-0x004A50C0: strings of
 * the debug menu's system and memory pages.
 * Data-only C (plan step 6b, docs/shifting.md); the pages that print them
 * are still separate files (named below). The strings are named globals
 * because the code files refer to them by name. The gaps around this range
 * are those files' own .rodata (jump tables, constants).
 */
/* rodata: 0x004A4D28-0x004A50C0 */

/* DbgSystemPage (4287B0.c) */
const char dbgSysStrSystem[] = "SYSTEM";
const char dbgSysStrIpAddress[] = "IP ADDRESS : %s";
const char dbgSysStrSceiLibrary214[] = "SCEI     Library 2.1.4";
const char dbgSysStrGa3dLibrary20001004[] = "GA 3D    Library 2000/10/04";
const char dbgSysStrGaSoundLibrary20001004[] = "GA SOUND Library 2000/10/04";
const char dbgSysStrFmtS[] = "%s";
const char dbgSysStrGsVersion[] = "GS Version   %x";
const char dbgSysStrGsVscfunc[] = "GS VSCfunc   %x";
const char dbgSysStrGsVscid[] = "GS VSCid     %x";
const char dbgSysStrGsIntermode[] = "GS InterMode %x";
const char dbgSysStrGsOutmode[] = "GS OutMode   %x";
const char dbgSysStrGsFfmode[] = "GS FFMode    %x";
const char dbgSysStrScreenMagH[] = "SCREEN MAG H %.3f";
const char dbgSysStrScreenMagV[] = "SCREEN MAG V %.3f";
const char dbgSysStrDebugMenuOn[] = "DEBUG MENU   ON";
const char dbgSysStrDebugMenuOff[] = "DEBUG MENU   OFF";
const char dbgSysStrT0Mode[] = "T0 Mode  %x";
const char dbgSysStrT0Count[] = "T0 Count %x";
const char dbgSysStrT0Comp[] = "T0 Comp  %x";
const char dbgSysStrT0Hold[] = "T0 Hold  %x";
const char dbgSysStrT1Mode[] = "T1 Mode  %x";
const char dbgSysStrT1Count[] = "T1 Count %x";
const char dbgSysStrT1Comp[] = "T1 Comp  %x";
const char dbgSysStrT1Hold[] = "T1 Hold  %x";
const char dbgSysStrT2Mode[] = "T2 Mode  %x";
const char dbgSysStrT2Count[] = "T2 Count %x";
const char dbgSysStrT2Comp[] = "T2 Comp  %x";
const char dbgSysStrT3Mode[] = "T3 Mode  %x";
const char dbgSysStrT3Count[] = "T3 Count %x";
const char dbgSysStrT3Comp[] = "T3 Comp  %x";

/* DbgMemoryPage (428D00.c) */
const char dbgMemStrMemory[] = "MEMORY";
const char dbgMemStrUseedMFreeMTotalM[] = "USEED %d.%d M / FREE %d.%d M / TOTAL %d.%d M";
const char dbgMemStrLibraryPartation[] = "<<LIBRARY PARTATION>>";
const char dbgMemStrFieldPartation[] = "<<FIELD PARTATION>>";
const char dbgMemStrBattlePartation[] = "<<BATTLE PARTATION>>";
const char dbgMemStrBattleMagicPartation[] = "<<BATTLE MAGIC PARTATION>>";
const char dbgMemStrDataPartation[] = "<<DATA PARTATION>>";
const char dbgMemStrLocalPartation[] = "<<LOCAL PARTATION>>";
const char dbgMemStrFacePartation[] = "<<FACE PARTATION>>";
const char dbgMemStrHandle[] = "Handle     %d";
const char dbgMemStrStat[] = "stat       %08x";
const char dbgMemStrAdrsStart[] = "adrs start %08x";
const char dbgMemStrEnd[] = "     end   %08x";
const char dbgMemStrSizeUsedM[] = "size used  %08x (%d.%dM)";
const char dbgMemStrFreeM[] = "     free  %08x (%d.%dM)";
const char dbgMemStrTotalM[] = "     total %08x (%d.%dM)";
