/* Third-party library code (newlib): see README.md, "Third-party library code". */
/* main executable .rodata 0x004A8448-0x004A8450: newlib's impure.c string
 * literal "C", the initial locale name of impure_data (struct _reent at
 * 0x004A0498, _current_locale at +0x34 = 0x004A04CC, which points here).
 * Data-only C (plan step 6b, docs/shifting.md).
 */
/* rodata: 0x004A8448-0x004A8450 */

const char __impureLocaleC[] = "C";
