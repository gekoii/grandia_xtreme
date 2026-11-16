/* main executable .rodata 0x004A1A68-0x004A1AE0: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A1A68.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A1A68-0x004A1AE0 */

/* Controller scale track page (DbgPtclCtrlScaleTrackPage 00404138). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char dbgPtclTrackSxSinStr[] = "SX_SIN"; /* 004A1A68 */
const char dbgPtclTrackSxMdStr[] = "SX_MD "; /* 004A1A70 */
const char dbgPtclTrackSxCtStr[] = "SX_CT "; /* 004A1A78 */
const char dbgPtclTrackSx0Str[] = "SX_0  "; /* 004A1A80 */
const char dbgPtclTrackSxRStr[] = "SX_R  "; /* 004A1A88 */
const char dbgPtclTrackSySinStr[] = "SY_SIN"; /* 004A1A90 */
const char dbgPtclTrackSyMdStr[] = "SY_MD "; /* 004A1A98 */
const char dbgPtclTrackSyCtStr[] = "SY_CT "; /* 004A1AA0 */
const char dbgPtclTrackSy0Str[] = "SY_0  "; /* 004A1AA8 */
const char dbgPtclTrackSyRStr[] = "SY_R  "; /* 004A1AB0 */
const char dbgPtclTrackVcMdStr[] = "VC_MD "; /* 004A1AB8 */
const char dbgPtclTrackVcCtStr[] = "VC_CT "; /* 004A1AC0 */
const char dbgPtclTrackVcolStr[] = "VCOL  "; /* 004A1AC8 */
const char dbgPtclTrackOnceStr[] = "ONCE  "; /* 004A1AD0 */
