/* main executable .data 0x0049C590-0x0049D2C8: the software mixer's pan
 * curves and per-ear delays. Data-only C (plan step 6b, docs/shifting.md);
 * the users are SndSoftVu0SetGains (0676E0.s, 4 floats at a time with
 * SndSoftVu0Gather4, hence 3 spare entries) and SndSoftMix (462528.c).
 * Indexed by a soft voice's pan, 0..0x100.
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x0049C590-0x0049D2C8 */

typedef signed char s8;
typedef float f32;

/* 1.0 up to [16], down to 0.08 at [48], back to 1.0 at [64]; the rest 0 */
f32 sndSoftPanCurve0[260] = {
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.94f, 0.86f, 0.78f, 0.73f, 0.68f, 0.62f, 0.56f,
    0.51f, 0.47f, 0.41f, 0.35f, 0.32f, 0.25f, 0.23f, 0.21f,
    0.2f, 0.19f, 0.17f, 0.16f, 0.15f, 0.14f, 0.14f, 0.13f,
    0.13f, 0.12f, 0.12f, 0.11f, 0.1f, 0.095f, 0.09f, 0.085f,
    0.08f, 0.1f, 0.16f, 0.22f, 0.27f, 0.33f, 0.38f, 0.43f,
    0.49f, 0.54f, 0.59f, 0.64f, 0.69f, 0.76f, 0.86f, 0.94f,
    1.0f,
};

/* 0 up to [16], peaks of 0.42 at [24] and 0.37 at [57]; the rest 0 */
f32 sndSoftPanCurve1[260] = {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.05f, 0.12f, 0.2f, 0.24f, 0.29f, 0.34f, 0.4f,
    0.42f, 0.39f, 0.34f, 0.29f, 0.26f, 0.2f, 0.18f, 0.17f,
    0.165f, 0.16f, 0.145f, 0.14f, 0.13f, 0.13f, 0.12f, 0.12f,
    0.11f, 0.11f, 0.1f, 0.1f, 0.09f, 0.08f, 0.07f, 0.06f,
    0.05f, 0.07f, 0.12f, 0.17f, 0.2f, 0.24f, 0.28f, 0.32f,
    0.37f, 0.37f, 0.34f, 0.31f, 0.27f, 0.21f, 0.12f, 0.05f,
};

/* 0 up to [16], rising to 0.87 at [48], falling to 0.01 at [63]; the rest 0 */
f32 sndSoftPanCurve2[260] = {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.01f, 0.02f, 0.02f, 0.03f, 0.03f, 0.04f, 0.04f,
    0.07f, 0.14f, 0.28f, 0.36f, 0.42f, 0.55f, 0.59f, 0.62f,
    0.64f, 0.67f, 0.69f, 0.71f, 0.72f, 0.73f, 0.74f, 0.75f,
    0.76f, 0.77f, 0.78f, 0.79f, 0.81f, 0.83f, 0.85f, 0.86f,
    0.87f, 0.84f, 0.72f, 0.61f, 0.53f, 0.43f, 0.34f, 0.25f,
    0.14f, 0.09f, 0.07f, 0.05f, 0.04f, 0.03f, 0.02f, 0.01f,
};

/* Sample delay of the left ear by pan (SndSoftMix); the right ear reads
 * [0x100 - pan] */
s8 sndPanDelayTable[257] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 8,
    9, 10, 11, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 12, 11, 10,
    9, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1,
    1, 1, 1, 1,
};
