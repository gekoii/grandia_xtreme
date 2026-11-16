/* BATTLE .rodata 0x006E23A8-0x006E23D0, inside the unit state file's
 * .rodata (after 6C8720.c's jump tables): the victory state's file names
 * (BtlUnitStateVictory 6C98A8.c, BtlVictoryMotionLoad 6C9DE0.c). Data-only
 * C (plan step 6b, docs/shifting.md).
 */
/* rodata: 0x006E23A8-0x006E23D0 */

const char btlVictorySeFile[] = "MUSIC/SYS_W.SE"; /* the SE bank loaded in the victory state */
const char btlVictoryDatSuffix[] = "_WIN.DAT"; /* the victory motion pack, BTL_DATA/P<id>_WIN.DAT (BtlCharFileName) */
