/* BATTLE .rodata 0x006E1860-0x006E1A20: the rest of the debug environment
 * pages' strings (shadow 6B7368.c, contrast 6B75C8.c), then the .rodata of
 * five more original files, each from its "save": the lens flare page
 * (6B7F30.c), the field page (6B8B88.c), one with nothing else, one with an
 * unused float table and BtlUnitsPreDraw's heap print (6B9470.c), and two
 * with nothing else. Data-only C (plan step 6b, docs/shifting.md).
 */
/* rodata: 0x006E1860-0x006E1A20 */

typedef float f32;

/* DbgEnvShadowPage */
const char dbgEnvShadowTitleFmt[] = "%sSHADOW%s page %d/2";
const char dbgEnvShadowRFmt[] = "R %.1f";
const char dbgEnvShadowGFmt[] = "G %.1f";
const char dbgEnvShadowBFmt[] = "B %.1f";
const char dbgEnvShadowAFmt[] = "A %.1f";
const char dbgEnvShadowCallbackFmt[] = "%.8x %.8x %.8x";
/* DbgEnvContrastPage */
const char dbgEnvContrastTitleFmt[] = "%sCONTRAST%s  map %d, env %d";
const char dbgEnvContrastAmbFmt[] = "AMB %.3f";
const char dbgEnvContrastDirFmt[] = "DIR %.3f";
static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
/* DbgEnvLensFlarePage */
const char dbgEnvFlareTitleFmt[] = "%sLENS FLARE%s";
const char dbgEnvFlareTitleColorStr[] = "\\c1 ";
const char dbgEnvFlareColor0Str[] = "\\c0 ";
const char dbgEnvFlareAlphaFmt[] = "A %.3f(x %.3f)";
const char dbgEnvFlareDistFmt[] = "DST %.1f";
const char dbgEnvFlareAngleXFmt[] = "RX 0x%.4x";
const char dbgEnvFlareAngleYFmt[] = "Y 0x%.4x";
const char dbgEnvFlareColor4Str[] = "\\c4 ";
const char dbgEnvFlarePosXFmt[] = "PX %7.1f";
const char dbgEnvFlarePosYFmt[] = "Y %7.1f";
const char dbgEnvFlarePosZFmt[] = "Z %7.1f";
static const char save_006E1978[] = "save"; /* the next file's .rodata starts here */
/* DbgEnvFieldPage */
const char dbgEnvFieldTitleFmt[] = "%sFIELD%s";
const char dbgEnvFieldTitleColorStr[] = "\\c1 ";
const char dbgEnvFieldColor0Str[] = "\\c0 ";
const char dbgEnvFieldClipStr[] = "CLIP     ";
const char dbgEnvFieldChkDispStr[] = "CHK_DISP ";
static const char save_006E19C0[] = "save"; /* the next file's .rodata starts here */
static const char save_006E19C8[] = "save"; /* the next file's .rodata starts here */
/* 0.5 to 1.5 in steps of 0.1; nothing refers to it */
static const f32 scaleSteps[11] = {
    0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f, 1.1f, 1.2f, 1.3f, 1.4f, 1.5f,
};
/* BtlUnitsPreDraw: the debug heap print */
const char btlStrHeapFree[] = "%6x/%6x";
static const char save_006E1A08[] = "save"; /* the next file's .rodata starts here */
static const char save_006E1A10[] = "save"; /* the next file's .rodata starts here */
