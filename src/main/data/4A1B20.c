/* main executable .rodata 0x004A1B20-0x004A1B50: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A1B1C.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A1B20-0x004A1B50 */

/* Controller rotation track page (DbgPtclCtrlRotTrackPage 00404528). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char dbgPtclTrackRdxStr[] = "RDX   "; /* 004A1B20 */
const char dbgPtclTrackRdxMdStr[] = "RDX_MD"; /* 004A1B28 */
const char dbgPtclTrackRdyStr[] = "RDY   "; /* 004A1B30 */
const char dbgPtclTrackRdyMdStr[] = "RDY_MD"; /* 004A1B38 */
const char dbgPtclTrackRdzStr[] = "RDZ   "; /* 004A1B40 */
const char dbgPtclTrackRdzMdStr[] = "RDZ_MD"; /* 004A1B48 */
