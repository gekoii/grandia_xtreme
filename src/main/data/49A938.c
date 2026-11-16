/* main executable .data 0x0049A938-0x0049A970: pad, GS packet and model
 * creation state. Data-only C (plan step 6b, docs/shifting.md); the users
 * are the pad code (PadReadPort, pad.h), 430AF0.c/430BB0.c (GS packets) and
 * 4381D8.c (ChrModelCreate). The next file (49A970.c) starts with a
 * 16-aligned DMA packet, which leaves the zeros after chrModelCreateUnused2.
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x0049A938-0x0049A970 */

typedef int s32;

/* pad.h, per port */
s32 padType[2] = { 0, 0 };      /* terminal type nibble (PadReadPort) */
s32 padLastType[2] = { 0, 0 };
s32 padPhase[2] = { 0, 0 };     /* mode setup state */

/* GS packets built on first use (430AF0, 430BB0) */
s32 gsZClearPktReady = 0;
s32 gsFbAlphaPktReady = 0;
void *gsPacketBuf = 0;          /* GsPacketBuf * */

/* cleared by ChrModelCreate (4381D8), never read */
s32 chrModelCreateUnused0 = 0;
s32 chrModelCreateUnused1 = 0;
s32 chrModelCreateUnused2 = 0;
