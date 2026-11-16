/* main executable: the start of a file's .rodata - the names of the debug
 * camera modes. They are the string literals of dbgCamModeNames (.data
 * 0x0049A690, in 49A660.c), which gcc emits after the table in reverse
 * order: here [4] first, [0] last. Data-only C (plan step 6b,
 * docs/shifting.md). Don't include the project headers here: declare what
 * the data refers to locally.
 */
/* rodata: 0x004A36A0-0x004A3730 */

/* A header's unused static string: the first thing in each file's .rodata */
static const char save[] = "save";

/* dbgCamModeNames[4..0], for dbgCamModes 0x36, 0x3A, 0x35, 0x33, 0x39 */
const char dbgCamModeStrPosEyeVec[] = " pos    + eye vector + dist";  /* [4] */
const char dbgCamModeStrPosWangle[] = " pos    + wangle + dist";      /* [3] */
const char dbgCamModeStrTargetEyeVec[] = " target + eye vector + dist";  /* [2] */
const char dbgCamModeStrTargetPos[] = " target + pos + dist ";        /* [1] */
const char dbgCamModeStrTargetWangle[] = " target + wangle + dist";      /* [0] */
