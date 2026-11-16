/*
 * The PS2's formats on x86-64 (port/docs/win64.md): included by grx_port.h
 * before every game source.
 *
 * Some of the game's structs are the hardware's layout: a DMA channel's
 * registers, the DMAC's ring settings. Their address fields are declared
 * as pointers, 4 bytes on the PS2 and on i686, 8 on x86-64, where every
 * field after one would land on the wrong register. On x86-64 those fields
 * are 32-bit EE addresses instead: the game's memory is below 2 GB, so an
 * address fits, and assigning a pointer to one keeps its low 32 bits (a
 * warning the game's flags allow, as for its other int/pointer mixes).
 *
 * GRX_EE_PTR(T) is such a field: T * on i686, a 32-bit integer on x86-64.
 * port/tools/fixups/win64.py puts it into the game's own local copies of
 * these structs. Nothing here changes the i686 build.
 */
#ifndef GRX_EE64_H
#define GRX_EE64_H

/* GRX_EE_TO(T, x): such a field (or any 32-bit EE address) as a T *, to
   go through it; a plain cast on i686, where the field is a T * already */
#define GRX_EE_TO(T, x) ((T *)(__UINTPTR_TYPE__)(x))

/* What port/tools/ee32.py writes into the x86-64 build's copy of the
   game's C, where every pointer field of a game struct is a GRX_EE32:
   GRX_EE_AS(P, x) reads one as the pointer type P, GRX_EE_FROM(v) is the
   32 bits a pointer is stored as, GRX_EE_ADD(P, f, d) and GRX_EE_POSTADD
   are f += d (++f) and f++ in P's elements. */
#define GRX_EE32 unsigned int
#define GRX_EE_AS(P, x) ((P)(__UINTPTR_TYPE__)(x))
#define GRX_EE_FROM(v) ((GRX_EE32)(__UINTPTR_TYPE__)(v))
#define GRX_EE_ADD(P, f, d) \
    ({ GRX_EE32 *grx_ee_a = (GRX_EE32 *)&(f); *grx_ee_a = GRX_EE_FROM(GRX_EE_AS(P, *grx_ee_a) + (d)); \
       GRX_EE_AS(P, *grx_ee_a); })
#define GRX_EE_POSTADD(P, f, d) \
    ({ GRX_EE32 *grx_ee_a = (GRX_EE32 *)&(f); __typeof__(GRX_EE_AS(P, 0)) grx_ee_o = GRX_EE_AS(P, *grx_ee_a); \
       *grx_ee_a = GRX_EE_FROM(grx_ee_o + (d)); grx_ee_o; })

#if __SIZEOF_POINTER__ == 4
#define GRX_EE_PTR(T) T *
#else
#define GRX_EE_PTR(T) unsigned int

/* sdk/libdma.h's types (it skips its own when SCE_DMA_TYPES is defined),
   with the address registers 32 bits wide */
#define SCE_DMA_TYPES
typedef struct tD_CHCR {
    unsigned int DIR : 1;
    unsigned int p0 : 1;
    unsigned int MOD : 2;
    unsigned int ASP : 2;
    unsigned int TTE : 1;
    unsigned int TIE : 1;
    unsigned int STR : 1;
    unsigned int p1 : 7;
    unsigned int TAG : 16;
} tD_CHCR;

typedef struct sceDmaChan {
    /* 0x00 */ tD_CHCR chcr;
    /* 0x04 */ unsigned int p0[3];
    /* 0x10 */ GRX_EE_PTR(void) madr;
    /* 0x14 */ unsigned int p1[3];
    /* 0x20 */ unsigned int qwc;
    /* 0x24 */ unsigned int p2[3];
    /* 0x30 */ GRX_EE_PTR(void) tadr;
    /* 0x34 */ unsigned int p3[3];
    /* 0x40 */ GRX_EE_PTR(void) as0;
    /* 0x44 */ unsigned int p4[3];
    /* 0x50 */ GRX_EE_PTR(void) as1;
    /* 0x54 */ unsigned int p5[3];
    /* 0x60 */ unsigned int p6[4];
    /* 0x70 */ unsigned int p7[4];
    /* 0x80 */ GRX_EE_PTR(void) sadr;
    /* 0x84 */ unsigned int p8[3];
} sceDmaChan;
_Static_assert(sizeof(sceDmaChan) == 0x90, "sceDmaChan is the DMAC's register layout");

typedef struct sceDmaEnv {
    /* 0x00 */ unsigned char sts;
    /* 0x01 */ unsigned char std;
    /* 0x02 */ unsigned char mfd;
    /* 0x03 */ unsigned char rcycle;
    /* 0x04 */ unsigned short express;
    /* 0x06 */ unsigned short notify;
    /* 0x08 */ unsigned short sqwc;
    /* 0x0A */ unsigned short tqwc;
    /* 0x0C */ GRX_EE_PTR(void) rbadr;
    /* 0x10 */ unsigned int rbmsk;
} sceDmaEnv;
_Static_assert(sizeof(sceDmaEnv) == 0x14, "sceDmaEnv is sceDmaPutEnv's layout");
#endif

#endif
