/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's sf_atan.c (atanf, 47ACC8.c),
 * sf_ceil.c, sf_cos.c and sf_floor.c. Data-only C (plan step 6b,
 * docs/shifting.md). Their functions load float constants with li.s, so
 * the fdlibm static scalars (one, huge) are unused, but gcc 2.9 still
 * emits them. Don't include the project headers here.
 */
/* rodata: 0x004A7858-0x004A78B8 */

/* sf_atan.c */
/* fields: D_004A7864 [3] (atan(inf)hi) */
const float atanhi[4] = {
    0.4636476f,  /* atan(0.5)hi 0x3EED6338 */
    0.7853981f,  /* atan(1.0)hi 0x3F490FDA */
    0.9827937f,  /* atan(1.5)hi 0x3F7B985E */
    1.5707963f,  /* atan(inf)hi 0x3FC90FDA */
};
/* fields: D_004A7874 [3] (atan(inf)lo) */
const float atanlo[4] = {
    5.0121582e-09f, /* atan(0.5)lo 0x31AC3769 */
    3.7748947e-08f, /* atan(1.0)lo 0x33222168 */
    3.4473217e-08f, /* atan(1.5)lo 0x33140FB4 */
    7.5497894e-08f, /* atan(inf)lo 0x33A22168 */
};
const float aT[11] = {
    0.33333334f,   /* 0x3EAAAAAB */
    -0.2f,         /* 0xBE4CCCCD */
    0.14285715f,   /* 0x3E124925 */
    -0.111111104f, /* 0xBDE38E38 */
    0.09090887f,   /* 0x3DBA2E6E */
    -0.07691876f,  /* 0xBD9D8795 */
    0.06661073f,   /* 0x3D886B35 */
    -0.058335703f, /* 0xBD6EF16B */
    0.04976878f,   /* 0x3D4BDA59 */
    -0.03653157f,  /* 0xBD15A221 */
    0.01628582f,   /* 0x3C8569D7 */
};
static const float atan_one = 1.0f;
static const float atan_huge = 1e+30f;

/* sf_ceil.c */
static const float ceil_huge = 1e+30f;

/* sf_cos.c */
static const float cos_one = 1.0f;

/* sf_floor.c */
static const float floor_huge = 1e+30f;
