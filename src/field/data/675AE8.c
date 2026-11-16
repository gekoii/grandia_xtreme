/* FIELD .rodata: the memory card space check's directory pattern.
 * Data-only C (plan step 6b, docs/shifting.md); the user is
 * McSpaceCheckReadDir (65CC00.c).
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* rodata: 0x00675AE8-0x00675B00 */

const char mcSpaceCheckDirPattern[] = "/BASLUS-20417GrandiaX/*"; /* 00675AE8 */
