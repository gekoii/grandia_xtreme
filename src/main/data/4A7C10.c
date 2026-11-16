/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's ef_fmod.c (__ieee754_fmodf,
 * 47D0A8.c). Data-only C (plan step 6b, docs/shifting.md). Split from
 * 4A7B80.c at the original file boundary. `one` is unused (folded into a
 * literal) but emitted; the function returns Zero[sign]. Don't include the
 * project headers here.
 */
/* rodata: 0x004A7C10-0x004A7C20 */

static const float one = 1.0f;
const float Zero[2] = { 0.0f, -0.0f };
