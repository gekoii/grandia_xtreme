/* main executable: the texts of the debug PAD, FILE and SOUND pages
 * (DbgPadPage 424E90, DbgFileIopStatDraw 425340, DbgFileInfoDraw 4254D8,
 * DbgFileCountsDraw 425670, DbgFileMenu 4256F8, DbgSoundBgmPage 425808,
 * DbgSoundSePage 425DB0; their other constants follow in their files). Part
 * of the .rodata of the file that 4A36A0.c starts. Data-only C (plan step
 * 6b, docs/shifting.md).
 */
/* rodata: 0x004A4230-0x004A47C0 */

/* DbgPadPage */
const char dbgPadStrType[] = "TYPE         %s";
const char dbgPadStrId[] = "ID           %04x";
const char dbgPadStrData[] = "DATA         %04x";
const char dbgPadStrDataf[] = "DATAF        %04x";
const char dbgPadStrDatar2[] = "datar2        %04x";
const char dbgPadStrLeftAnlgH[] = "LEFT  ANLG H %04x";
const char dbgPadStrLeftAnlgV[] = "LEFT  ANLG V %04x";
const char dbgPadStrRightAnlgH[] = "RIGHT ANLG H %04x";
const char dbgPadStrRightAnlgV[] = "RIGHT ANLG V %04x";
const char dbgPadStrRightMotor[] = "RIGHT motor  %04x";
const char dbgPadStrLeftMotor[] = "LEFT  motor  %04x";
const char dbgPadStrPressModeViewer[] = "PRESS MODE VIEWER";
const char dbgPadStrIndexValue[] = "%2d : %d";
const char dbgPadStrCursor[] = ">";

/* DbgFileIopStatDraw: stream driver status words */
const char dbgFileIopStrIopStat[] = "iop_stat ";
const char dbgFileIopStrStat[] = "stat    %08x %08x %08x";
const char dbgFileIopStrSecnum[] = "secnum  %08x %08x %08x";
const char dbgFileIopStrGetofs[] = "getofs  %08x %08x %08x";
const char dbgFileIopStrBufofs[] = "bufofs  %08x %08x %08x";
const char dbgFileIopStrBuflen[] = "buflen  %08x %08x %08x";
const char dbgFileIopStrDummy[] = "dummy   %08x %08x %08x";
const char dbgFileIopStrReadlen[] = "readlen %08x %08x %08x";
const char dbgFileIopStrBufadr[] = "bufadr  %08x %08x %08x";
const char dbgFileIopStrReqlen[] = "reqlen  %08x %08x %08x";
const char dbgFileIopStrLsn[] = "lsn     %08x %08x %08x";

/* DbgFileInfoDraw */
const char dbgFileInfoStrFileName[] = "File Name %s";
const char dbgFileInfoStrFlag[] = "flag      %d";
const char dbgFileInfoStrMode[] = "mode      %d";
const char dbgFileInfoStrHandle[] = "handle    %d";
const char dbgFileInfoStrAddress[] = "address  $%x";
const char dbgFileInfoStrReadLen[] = "read len  %d";
const char dbgFileInfoStrReadSect[] = "read sect %d";
const char dbgFileInfoStrFileLen[] = "file len  %d";
const char dbgFileInfoStrFileSect[] = "file_sect %d";
const char dbgFileInfoStrCdbufLen[] = "cdbuf len %d";
const char dbgFileInfoStrOpsHeader[] = " open read check req  get close ";
const char dbgFileInfoStrOpsHex[] = "%4x %4x %4x %4x %4x %4x";

/* DbgFileCountsDraw */
const char dbgFileCntStrOps[] = "%4d %4d %4d %4d %4d %4d";

/* DbgFileMenu: page %d of 3 between dbgStrPageHiliteOn/Off */
const char dbgFileMenuStrFileMenuPage3[] = "FILE MENU page %s %d/3%s ";

/* DbgSoundBgmPage: the request, then the VMSND driver state */
const char dbgSndBgmStrReqNo[] = "REQ NO  : %d";
const char dbgSndBgmStrReqFade[] = "FADE    : %d";
const char dbgSndBgmStrReqVolume[] = "VOLUME  : %.3f%%";
const char dbgSndBgmStrProcess[] = "PROCESS : %3d %3d";
const char dbgSndBgmStrReqStop[] = "REQ STOP: %3d %3d";
const char dbgSndBgmStrVolume[] = "VOLUME  : %3x %3x";
const char dbgSndBgmStrFade[] = "FADE    : %3d %3d";
const char dbgSndBgmStrVmsndIndex[] = "VMSND INDEX";
const char dbgSndBgmStrIndex[] = "INDEX   : %x";
const char dbgSndBgmStrInitdata[] = "INITDATA: %x";
const char dbgSndBgmStrInitsize[] = "INITSIZE: %x";
const char dbgSndBgmStrVmsndOpen[] = "VMSND OPEN";
const char dbgSndBgmStrOpenObjnum[] = "OBJNUM  : %4x %4x";
const char dbgSndBgmStrOpenIndexnum[] = "INDEXNUM: %4d %4d";
const char dbgSndBgmStrHandle[] = "HANDLE  : %4x %4x";
const char dbgSndBgmStrSeclen[] = "SECLEN  : %4x %4d";
const char dbgSndBgmStrVmsndStat[] = "VMSND STAT";
const char dbgSndBgmStrStat[] = "STAT    : %x";
const char dbgSndBgmStrAtr[] = "ATR     : %x";
const char dbgSndBgmStrCate[] = "CATE    : %x";
const char dbgSndBgmStrError[] = "ERROR   : %x";
const char dbgSndBgmStrNowvol[] = "NOWVOL  : %x";
const char dbgSndBgmStrDisvol[] = "DISVOL  : %x";
const char dbgSndBgmStrKtime[] = "KTIME   : %x";
const char dbgSndBgmStrVmsndOn1[] = "VMSND ON1";
const char dbgSndBgmStrOn1Objnum[] = "OBJNUM  : %x";
const char dbgSndBgmStrOn1Indexnum[] = "INDEXNUM: %d";
const char dbgSndBgmStrDist[] = "DIST    : %x";
const char dbgSndBgmStrHpan[] = "HPAN    : %x";
const char dbgSndBgmStrVpan[] = "VPAN    : %x";

/* DbgSoundSePage: the state names of an SE slot, copied into a local
 * (probably a local array initializer, which gcc 2.9 puts in .rodata).
 * The bytes "LAY\0" of "PLAY" (0x004A47B0) look like a .bss address
 * (gphProjTexBuf + 0x1C9CC): config/reloc_addrs.txt makes splat write
 * them as a number (MIPS_NONE). */
const char dbgSndSeFlagNames[5][5] = {
    "NONE", "REQU", "OPEN", "PLAY", "MOVE",
};
