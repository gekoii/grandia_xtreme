/* FIELD .data: the auto-map item roll log position and the inventory
 * capacities. Data-only C (plan step 6b, docs/shifting.md); the users are
 * 614FC8.c (the roll log), ItemWorkInit (5EAA58.c) and ItemUpdateLimits
 * (5EAD80.c, sets the capacities: 30 items, 12 or 24 eggs and books, 50 or
 * 100 skills).
 * Split from the task's 0x00666788-0x00666F90: the objects end at
 * 0x00666CDC and the zeros up to 0x00666CE0 are what the next file's
 * 16-aligned start recreates.
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x00666CD0-0x00666CE0 */

typedef signed short s16;
typedef signed int s32;

s32 amapItemLogPos = 0;    /* 00666CD0: ring index (0-31) of the item roll log */
s16 itemMaxItems = 0;      /* 00666CD4 */
s16 itemEggCapacity = 0;   /* 00666CD6 */
s16 itemBookCapacity = 0;  /* 00666CD8 */
s16 itemSkillCapacity = 0; /* 00666CDA */
