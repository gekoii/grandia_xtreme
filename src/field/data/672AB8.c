/* FIELD .rodata 0x00672AB8-0x00672D50: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C2AB8.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00672AB8-0x00672D50 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* Format strings of the OCC debug page (DbgFldOccPage, src/field/5DAB98.c). */

const char dbgOccStrNumber[] = "OCC Number: %d / %d"; /* 00672AB8 */
const char dbgOccStrCat[] = "OCC cat: %d"; /* 00672AD0 */
const char dbgOccStrGrp[] = "Grp: %d"; /* 00672AE0 */
const char dbgOccStrIndex[] = "Index: %d"; /* 00672AE8 */
const char dbgOccStrEnabledOn[] = "Enabled   : ON"; /* 00672AF8 */
const char dbgOccStrEnabledOff[] = "Enabled   : OFF"; /* 00672B08 */
const char dbgOccStrDisabledOn[] = "Disabled  : ON"; /* 00672B18 */
const char dbgOccStrDisabledOff[] = "Disabled  : OFF"; /* 00672B28 */
const char dbgOccStrType[] = "Type      : %d"; /* 00672B38 */
const char dbgOccStrFirstWait[] = "FirstWait : %d"; /* 00672B48 */
const char dbgOccStrBeginWait[] = "BeginWait : %d"; /* 00672B58 */
const char dbgOccStrStatus[] = "Status   : %s"; /* 00672B68 */
const char dbgOccStrCount[] = "Count    : %d"; /* 00672B78 */
const char dbgOccStrLoopOn[] = "Loop     : ON"; /* 00672B88 */
const char dbgOccStrLoopOff[] = "Loop     : OFF"; /* 00672B98 */
const char dbgOccStrAutoWakeOn[] = "AutoWake : ON"; /* 00672BA8 */
const char dbgOccStrAutoWakeOff[] = "AutoWake : OFF"; /* 00672BB8 */
const char dbgOccStrAttribute[] = "Attribute: %04X"; /* 00672BC8 */
const char dbgOccStrTargetId[] = "Target ID : %d"; /* 00672BD8 */
const char dbgOccStrMethod[] = "Method    : %d"; /* 00672BE8 */
const char dbgOccStrProgress[] = "progress  : %d"; /* 00672BF8 */
const char dbgOccStrFwBeginVec[] = "fw_begin  : (%4.2f, %4.2f, %4.2f)"; /* 00672C08 */
const char dbgOccStrVwDeltaVec[] = "vw_delta  : (%4.2f, %4.2f, %4.2f)"; /* 00672C30 */
const char dbgOccStrVwProgressPair[] = "vw_progress : (%d, %d)"; /* 00672C58 */
const char dbgOccStrFwDecay[] = "fw_decay  : %4.2f"; /* 00672C70 */
const char dbgOccStrVwDir[] = "vw_dir    : (%d, %d, %d)"; /* 00672C88 */
const char dbgOccStrAlpha[] = "alpha      : %d, %d"; /* 00672CA8 */
const char dbgOccStrCountPair[] = "count      : %d, %d"; /* 00672CC0 */
const char dbgOccStrHalfWait[] = "half_wait  : %d"; /* 00672CD8 */
const char dbgOccStrEndWait[] = "end_wait   : %d"; /* 00672CE8 */
const char dbgOccStrFwBegin[] = "fw_begin   : %d"; /* 00672CF8 */
const char dbgOccStrFwEnd[] = "fw_end     : %d"; /* 00672D08 */
const char dbgOccStrVwStatus[] = "vw_status  : %d"; /* 00672D18 */
const char dbgOccStrVwProgress[] = "vw_progress: %d"; /* 00672D28 */
const char dbgOccStrVwDelta[] = "vw_delta   : %d"; /* 00672D38 */
