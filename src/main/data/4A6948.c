/* main executable .rodata 0x004A6948-0x004A6970: VuVecLerp4's (1, 1, 1, 1)
 * and ChrFaceAnimUpdate's blink sequence. Data-only C (plan step 6b,
 * docs/shifting.md). vuOneVec4 is a 16-aligned VU0 vector; it can't be
 * declared aligned(16) here, because the range starts 8 bytes before it, so
 * those 8 zero bytes are an object of their own (D_004A6948, nothing refers
 * to it). The word before the range (D_004A6944, zero padding) stays a small
 * asm piece.
 */
/* rodata: 0x004A6948-0x004A6970 */

typedef unsigned char u8;
typedef unsigned int u32;
typedef float f32;

const u32 D_004A6948[2] = { 0, 0 };

/* VuVecLerp4 (0x00452A60, unused, still asm): out = a + (b - a) * t */
const f32 vuOneVec4[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

/* ChrFaceAnimUpdate (453CB0.c) copies both as 3-byte structs: a blink is 3
 * steps of chrBlinkStepFrames[i] frames showing face chrBlinkStepFaces[i].
 * Probably a local array's initializer in the original. */
const u8 chrBlinkStepFrames[3] = { 7, 4, 3 };
const u8 chrBlinkStepFaces[3] = { 0, 2, 1 };
