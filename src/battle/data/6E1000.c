/* BATTLE .rodata 0x006E1000-0x006E10C0, inside the original file that
 * starts at 0x006E0F90 (after 6A9088.c's jump tables): 16 bytes nothing
 * refers to, then the strings of the debug info page DbgBtlInfoPage
 * (6AA078.c). Data-only C (plan step 6b, docs/shifting.md).
 */
/* rodata: 0x006E1000-0x006E10C0 */

typedef unsigned char u8;

/* Two rows of 8 byte flags; nothing refers to them */
const u8 D_006E1000[2][8] = {
    { 0, 1, 0, 1, 0, 0, 0, 0 },
    { 1, 1, 1, 1, 0, 0, 0, 0 },
};

/* DbgBtlInfoPage: title, highlight colour codes, the HUD toggles, then the
 * IP dial position, scale and the dial values */
const char dbgInfoStrTitle[] = "%sINFO%s";
const char dbgInfoStrColHi[] = "\\c1 ";
const char dbgInfoStrColLo[] = "\\c0 ";
const char dbgInfoStrHp[] = "HP  ";
const char dbgInfoStrIp[] = "IP  ";
const char dbgInfoStrDmg[] = "DMG ";
const char dbgInfoStrCmr[] = "CMR ";
const char dbgInfoStrRepo[] = "REPO";
const char dbgInfoStrIpX[] = "IP X %d";
const char dbgInfoStrIpY[] = "IP Y %d";
const char dbgInfoStrSx[] = "  SX %.1f";
const char dbgInfoStrSy[] = "  SY %.1f";
const char dbgInfoStrAng[] = " ANG %.4X";
const char dbgInfoStrRate[] = "RATE %.3f";
const char dbgInfoStrDelta[] = "   D %.3f";
const char dbgInfoStrCng[] = " CNG %d";
