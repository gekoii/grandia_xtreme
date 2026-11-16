/* BATTLE: the start of the .rodata of the file with BtlFormationFindSpot
 * (6BE858.c): its candidate spots. Data-only C (plan step 6b,
 * docs/shifting.md). Don't include the project headers here: declare what
 * the data refers to locally.
 */
/* rodata: 0x006E1D58-0x006E1E30 */

typedef unsigned int u32;
typedef float f32;

typedef struct {
    f32 x;
    f32 z;
} Pos2;

/* 8 zero bytes before the file's "save" (16-aligned); nothing refers to
 * them */
const u32 D_006E1D58[2] = { 0, 0 };

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */

/* 5 rows of 5 {x, z} spots, scored by BtlFormationFindSpot: rows 0-3 one
 * quadrant each, at radius 630 (15, 45, 75, 22.5 and 67.5 degrees from the
 * quadrant's start); row 4 the axes at radius 210, then the centre */
const Pos2 btlFormationSpots[25] = {
    { 163.17f, 608.58f }, { 445.41f, 445.41f }, { 608.58f, 163.17f }, { 241.29f, 582.12f }, { 582.12f, 241.29f },
    { 608.58f, -163.17f }, { 445.41f, -445.41f }, { 163.17f, -608.58f }, { 582.12f, -241.29f }, { 241.29f, -582.12f },
    { -163.17f, -608.58f }, { -445.41f, -445.41f }, { -608.58f, -163.17f }, { -241.29f, -582.12f }, { -582.12f, -241.29f },
    { -608.58f, 163.17f }, { -445.41f, 445.41f }, { -163.17f, 608.58f }, { -582.12f, 241.29f }, { -241.29f, 582.12f },
    { 0.0f, 210.0f }, { 210.0f, 0.0f }, { 0.0f, -210.0f }, { -210.0f, 0.0f }, { 0.0f, 0.0f },
};
