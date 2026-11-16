/* main executable .rodata 0x004A55F8-0x004A5770: strings of
 * the debug menu's texture info page.
 * Data-only C (plan step 6b, docs/shifting.md); the pages that print them
 * are still separate files (named below). The strings are named globals
 * because the code files refer to them by name. The gaps around this range
 * are those files' own .rodata (jump tables, constants).
 */
/* rodata: 0x004A55F8-0x004A5770 */

/* DbgTexturePage (42B440.c) */
const char dbgTexStrTextureInfo[] = "TEXTURE INFO";
const char dbgTexStrTexNo[] = "TEX NO %d / %d";
const char dbgTexStrId[] = "ID     %d";
const char dbgTexStrWidth[] = "WIDTH  %d";
const char dbgTexStrHeight[] = "HEIGHT %d";
const char dbgTexStrFormat16[] = "FORMAT 16";
const char dbgTexStrFormat256[] = "FORMAT 256";
const char dbgTexStrFormatFull[] = "FORMAT Full";
const char dbgTexStrFormat24bit[] = "FORMAT 24bit";
const char dbgTexStrFormat16bit[] = "FORMAT 16bit";
const char dbgTexStrUnSurpport[] = "Un Surpport";
const char dbgTexStrMipmap[] = "MIPMAP %d";
const char dbgTexStrClut[] = "CLUT   %d";
const char dbgTexStrLineraT[] = "LINERA T";
const char dbgTexStrLineraB[] = "LINERA B";
const char dbgTexStrSpecularOn[] = "SPECULAR ON";
const char dbgTexStrSpecularOff[] = "SPECULAR OFF";
const char dbgTexStrMipmapL[] = "MIPMAP L %d";
const char dbgTexStrMipmapK[] = "MIPMAP K %.3f";
const char dbgTexStrAllMipmapL[] = "ALL MIPMAP L %d";
const char dbgTexStrAllMipmapK[] = "ALL MIPMAP K %.3f";
const char dbgTexStrAllLineraT[] = "ALL LINERA T";
const char dbgTexStrAllLineraB[] = "ALL LINERA B";
