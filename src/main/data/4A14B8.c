/* main executable .rodata 0x004A14B8-0x004A15B0: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A14B4.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A14B8-0x004A15B0 */

/* Debug menu root (DbgDrawCursor 00401250, DbgMenuUpdate 004014F0). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
const char dbgCursorOnStr[] = "\\c8 >\\c7 "; /* 004A14C0 */
const char dbgCursorOffStr[] = "\\c8 -\\c7 "; /* 004A14D0 */
const char dbgMenuTitleFmt[] = "\\w30 %sDEBUG MENU%s \\w12"; /* 004A14E0 */
const char dbgMenuTitleColorStr[] = "\\c1 "; /* 004A1500 */
const char dbgMenuColor0Str[] = "\\c0 "; /* 004A1508 */
const char dbgMenuApplicationStr[] = ">APPRICATION"; /* 004A1510 */
const char dbgMenuSystemStr[] = "+SYSTEM"; /* 004A1520 */
const char dbgMenuEnvironmentStr[] = ">ENVIROMENT"; /* 004A1528 */
const char dbgMenuObjectStr[] = "+OBJECT"; /* 004A1538 */
const char dbgMenuFbEffectStr[] = ">FBEFCT"; /* 004A1540 */
const char dbgMenuCameraStr[] = ">CAMERA"; /* 004A1548 */
const char dbgMenuMotionStr[] = ">MOTION"; /* 004A1550 */
const char dbgMenuScreenDivStr[] = ">SCREENDIV"; /* 004A1558 */
const char dbgMenuPointLightStr[] = ">POINT LIGHT"; /* 004A1568 */
const char dbgMenuWipeStr[] = ">WIPE"; /* 004A1578 */
const char dbgMenuFadeStr[] = ">FADE"; /* 004A1580 */
const char dbgMenuParticleStr[] = "+PARTICLE"; /* 004A1588 */
const char dbgMenuTextureStr[] = ">TEXTURE"; /* 004A1598 */
