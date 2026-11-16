/* FIELD .rodata 0x006725B8-0x006729F0: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C25B8.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x006725B8-0x006729F0 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* Format strings of the second XVS information page (DbgFldXvsCharaPage,
 * src/field/5DA218.c) and the game debug menu (DbgFldGameDebugMenu, 5DA4D8). */

const char dbgXvsStrInfo2[] = "XVS INFORMATION 2"; /* 006725B8 */
const char dbgXvsStrLutina[] = "CHR Lutina   : %3d %3d"; /* 006725D0 */
const char dbgXvsStrMyamm[] = "CHR Myamm    : %3d %3d"; /* 006725E8 */
const char dbgXvsStrBrandol[] = "CHR Brandol  : %3d %3d"; /* 00672600 */
const char dbgXvsStrCarmyne[] = "CHR Carmyne  : %3d %3d"; /* 00672618 */
const char dbgXvsStrJaid[] = "CHR Jaid     : %3d %3d"; /* 00672630 */
const char dbgXvsStrTitto[] = "CHR Titto    : %3d %3d"; /* 00672648 */
const char dbgXvsStrUlk[] = "CHR Ulk      : %3d %3d"; /* 00672660 */
const char dbgXvsStrBoy[] = "CHR boy      : %3d %3d"; /* 00672678 */
const char dbgXvsStrOkusan[] = "CHR okusan   : %3d %3d"; /* 00672690 */
const char dbgXvsStrOssan[] = "CHR ossan    : %3d %3d"; /* 006726A8 */
const char dbgXvsStrWaitress[] = "CHR waitress : %3d %3d"; /* 006726C0 */
const char dbgXvsStrInu[] = "CHR inu      : %3d %3d"; /* 006726D8 */
const char dbgXvsStrMagic1[] = "CHR magic 1  : %3d %3d"; /* 006726F0 */
const char dbgXvsStrMagic2[] = "CHR magic 2  : %3d %3d"; /* 00672708 */
const char dbgXvsStrMuda[] = "CHR muda     : %3d %3d"; /* 00672720 */
const char dbgXvsStrSkill[] = "CHR skill    : %3d %3d"; /* 00672738 */
const char dbgGameDebugStrTitle[] = "GAME DEBUG Page %s %d / 26 %s "; /* 00672750 */
const char dbgGameDebugStrMainFlag[] = "MAIN FLAG AND COUNTER"; /* 00672770 */
const char dbgGameDebugStrGlobalFlag[] = "< GLOBAL  > FLAG %d"; /* 00672788 */
const char dbgGameDebugStrSubGameFlag[] = "< SUBGAME > FLAG %d"; /* 006727A0 */
const char dbgGameDebugStrXvsFlag[] = "< XVS     > FLAG %d"; /* 006727B8 */
const char dbgGameDebugStrSystemFlag[] = "< SYSTEM  > FLAG %d"; /* 006727D0 */
const char dbgGameDebugStrLocalFlag[] = "< LOCAL   > FLAG %d"; /* 006727E8 */
const char dbgGameDebugStrTreasureFlag[] = "< TRESURE > FLAG %d"; /* 00672800 */
const char dbgGameDebugStrMonsterFlag[] = "< MONSTER > FLAG %d"; /* 00672818 */
const char dbgGameDebugStrDungeonFlag[] = "< DUNGEON > FLAG %d"; /* 00672830 */
const char dbgGameDebugStrGameFlag[] = "< GAME    > FLAG %d"; /* 00672848 */
const char dbgGameDebugStrAreaMapFlag[] = "< AREAMAP > FLAG %d"; /* 00672860 */
const char dbgGameDebugStrReaderFlag[] = "< READER  > FLAG %d"; /* 00672878 */
const char dbgGameDebugStrTalkCounter[] = "< TALK >       COUNTER %d"; /* 00672890 */
const char dbgGameDebugStrPointTalkCounter[] = "< POINT TALK > COUNTER %d"; /* 006728B0 */
const char dbgGameDebugStrLocalCounter[] = "< LOCAL >      COUNTER %d"; /* 006728D0 */
const char dbgGameDebugStrGlobalCounter[] = "< GLOBAL >     COUNTER %d"; /* 006728F0 */
const char dbgGameDebugStrMealCounter[] = "< MEAL >       COUNTER %d"; /* 00672910 */
const char dbgGameDebugStrSubTime1Counter[] = "< SUB TIME 1 > COUNTER %d"; /* 00672930 */
const char dbgGameDebugStrSubTime2Counter[] = "< SUB TIME 2 > COUNTER %d"; /* 00672950 */
const char dbgGameDebugStrXvsCounter[] = "< XVS >        COUNTER %d"; /* 00672970 */
const char dbgGameDebugStrSubGameCounter[] = "< SUB GAME >   COUNTER %d"; /* 00672990 */
const char dbgGameDebugStrFaceList[] = "FACE LIST  DISP"; /* 006729B0 */
const char dbgGameDebugStrFaceTable[] = "FACE TABLE DISP"; /* 006729C0 */
const char dbgGameDebugStrPartyMember[] = "PARTY MEMBER LIST"; /* 006729D0 */
