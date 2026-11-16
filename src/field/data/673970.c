/* FIELD .rodata 0x00673970-0x00673AD0: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C396C.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00673970-0x00673AD0 */

typedef unsigned short u16;

/* {min, max} numbers that DbgParamItemGetPage (5EDDD8) steps through;
 * it copies the whole struct (src/field/5EDDD8.c: Ranges). */
typedef struct {
    u16 min;
    u16 max;
} Range;

typedef struct {
    Range r[4];
} Ranges;

const Ranges dbgParamItemGetRanges = {{
    {101, 2000}, /* item */
    {301, 344},  /* egg */
    {501, 507},  /* book */
    {1, 66},     /* skill */
}}; /* 00673970 */

/* DbgParamItemGetPage's strings */
const char dbgParamStrItemNo[] = "ITEM  NO = %4d"; /* 00673980 */
const char dbgParamStrEggNo[] = "EGG   NO = %4d"; /* 00673990 */
const char dbgParamStrBookNo[] = "BOOK  NO = %4d"; /* 006739A0 */
const char dbgParamStrSkillNo[] = "SKILL NO = %4d"; /* 006739B0 */
const char dbgParamStrLeaveItem[] = "LEAVE ITEM"; /* 006739C0 */
const char dbgParamStrGold[] = "GOLD = %d"; /* 006739D0 */
const char dbgParamStrTempMode[] = "temp_mode  = %d\n"; /* 006739E0 */
const char dbgParamStrTempMode2[] = "temp_mode2 = %d\n"; /* 006739F8 */
const char dbgParamStrTempNo[] = "temp_no[%d] = %d\n"; /* 00673A10 */
const char dbgParamStrItemOk[] = "ITEM GET OK!!"; /* 00673A28 */
const char dbgParamStrItemErr[] = "ITEM GET ERROR!!"; /* 00673A38 */
const char dbgParamStrEggOk[] = "EGG GET OK!!"; /* 00673A50 */
const char dbgParamStrEggErr[] = "EGG GET ERROR!!"; /* 00673A60 */
const char dbgParamStrBookOk[] = "BOOK GET OK!!"; /* 00673A70 */
const char dbgParamStrBookErr[] = "BOOK GET ERROR!!"; /* 00673A80 */
const char dbgParamStrSkillOk[] = "SKILL GET OK!!"; /* 00673A98 */
const char dbgParamStrSkillErr[] = "SKILL GET ERROR!!"; /* 00673AA8 */
const char dbgParamStrLeaveOk[] = "LEAVE OK !!"; /* 00673AC0 */
