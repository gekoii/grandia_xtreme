/* FIELD .rodata 0x00671540-0x006715A8: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C1540.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00671540-0x006715A8 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
/* The OCC/OCCS status names, in reverse order of the .data table that points
 * at them, dbgFldOccStatusNames (0x00666AD0: Sleep, Wake, FirstWait,
 * BeginWait, Forward, BackwardWait, Backward, End): gcc emits a table's
 * literals last element first, so the table and these strings come from one
 * source file (the OCC debug page, "Status   : %s"). The table is in
 * src/field/data/666788.c (batch 6b-12), so they stay named here. */
const char dbgFldOccStatusStrEnd[] = "End"; /* 00671548 */
const char dbgFldOccStatusStrBackward[] = "Backward"; /* 00671550 */
const char dbgFldOccStatusStrBackwardWait[] = "BackwardWait"; /* 00671560 */
const char dbgFldOccStatusStrForward[] = "Forward"; /* 00671570 */
const char dbgFldOccStatusStrBeginWait[] = "BeginWait"; /* 00671578 */
const char dbgFldOccStatusStrFirstWait[] = "FirstWait"; /* 00671588 */
const char dbgFldOccStatusStrWake[] = "Wake"; /* 00671598 */
const char dbgFldOccStatusStrSleep[] = "Sleep"; /* 006715A0 */
