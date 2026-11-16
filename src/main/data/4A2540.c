/* main executable .rodata 0x004A2540-0x004A2550: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A253C.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A2540-0x004A2550 */

/* GRX.INI loader (SysIniLoad 0040D860). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
const char sysIniFileName[] = "GRX.INI"; /* 004A2548 */
