/* main executable: the start of a file's .rodata - the colours and offsets
 * of the shadowed layers of the demo screen's text images
 * (SysDemoPromptPrims 41E3F0, SysDemoCaptionPrims 41E710). Each function
 * copies its two tables into locals (they were probably local array
 * initializers, which gcc 2.9 puts in .rodata). Data-only C (plan step 6b,
 * docs/shifting.md). Don't include the project headers here: declare what
 * the data refers to locally.
 */
/* rodata: 0x004A3400-0x004A3430 */

typedef unsigned char u8;

/* A header's unused static string: the first thing in each file's .rodata */
static const char save[] = "save";

/* SysDemoPromptPrims: 3 layers (shadows, then the text) */
const u8 sysDemoPromptColors[3][4] = {  /* r, g, b, a */
    { 0xF0, 0x40, 0x40, 0x40 },
    { 0xF0, 0x40, 0x40, 0x40 },
    { 0x80, 0x80, 0x80, 0x80 },
};
const signed char sysDemoPromptOffsets[3] = { -2, 2, 0 };  /* x */

/* SysDemoCaptionPrims: 2 layers (shadow, then the text) */
const u8 sysDemoCaptionColors[2][4] = {  /* r, g, b, a */
    { 0x00, 0x00, 0x00, 0x40 },
    { 0x80, 0x80, 0x80, 0x80 },
};
const signed char sysDemoCaptionOffsets[2] = { 3, 0 };  /* x and y */
