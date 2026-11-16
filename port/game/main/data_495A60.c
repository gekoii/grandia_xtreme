/* Port-only stand-ins for the VU microcode blocks (vuMicroCode0 0x00495A60,
   vuMicroCode1, vuMicroCode1End), which the decompilation still has as data
   in assembly. */
/*
 * On the PS2 these are VIF packets: MPG codes carrying the VU0 and VU1
 * microprograms (port/docs/vu.md). In the port both microprograms are C
 * (port/src/vu0_micro.c, port/src/vu1/), so the code itself isn't needed.
 * The blocks still have to exist with their sizes and in their order,
 * because the game uploads them by address arithmetic:
 *   VuMicroLoadVu0: vuMicroCode0, (vuMicroCode1 - vuMicroCode0) / 16 qwords
 *   VuMicroLoadVu1: vuMicroCode1, (vuMicroCode1End - vuMicroCode1) >> 4
 * So they are one block, mostly zero (VIF NOPs). vuMicroCode1 ends as the
 * original does: an UNPACK of two quadwords to VU1 data address 0, then
 * MSCAL 0, the init program, which reads them as vf31 = (0, 0, 0, 0) and
 * vf29 = (0, 0, 255, 0) (port/src/vu1/vu1.c).
 */
#include "common.h"

#define VU0_SIZE 0xF10
#define VU1_SIZE 0x3100

/* The init tail: UNPACK V4-32, 2 vectors to 0; the two quadwords; MSCAL 0 */
#define VIF_UNPACK_V4_32(num, addr) (0x6C000000 | ((num) << 16) | (addr))
#define VIF_MSCAL(addr) (0x14000000 | (addr))
#define F_255 0x437F0000 /* 255.0f */

#define TAIL_WORDS 10

typedef struct {
    u8 vu0[VU0_SIZE];
    u8 vu1[VU1_SIZE - TAIL_WORDS * 4];
    u32 tail[TAIL_WORDS];
    u8 end[0x90]; /* vuMicroCode1End: as large as the original object */
} VuMicroCodeBlocks;

VuMicroCodeBlocks vuMicroCodeBlocks __attribute__((aligned(16))) = {
    {0},
    {0},
    {VIF_UNPACK_V4_32(2, 0), 0, 0, 0, 0, 0, 0, F_255, 0, VIF_MSCAL(0)},
    {0},
};

/* The three names, at their offsets in the block */
#define GRX_STR2(x) #x
#define GRX_STR(x) GRX_STR2(x)
#define SYM(name) GRX_STR(__USER_LABEL_PREFIX__) #name
GRX_HOST_ASM(".globl " SYM(vuMicroCode0) "\n.set " SYM(vuMicroCode0) ", " SYM(vuMicroCodeBlocks) "\n"
        ".globl " SYM(vuMicroCode1) "\n.set " SYM(vuMicroCode1) ", " SYM(vuMicroCodeBlocks) "+0xF10\n"
        ".globl " SYM(vuMicroCode1End) "\n.set " SYM(vuMicroCode1End) ", " SYM(vuMicroCodeBlocks) "+0x4010");
