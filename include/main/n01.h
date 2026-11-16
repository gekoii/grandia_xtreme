#ifndef MAIN_N01_H
#define MAIN_N01_H

/*
 * Main executable 0x004000C0-0x004172E0: main loop and program manager,
 * debug menu, particle starter runtime and its debug pages, misc helpers,
 * screen shatter, face portraits, 2D sprites, party parameters and the
 * bitmap font. Globals here are used only by these files; the shared ones
 * (and the sprite and text records) are in sys.h, pad.h, gph.h, ptcl.h,
 * party.h and grxsys.h.
 */

#include "common.h"
#include "gph.h"   /* GphBatchQuad, GphQuad3D */
#include "sys.h"   /* SprRect, TxtGlyphBuf, TxtObj (battle/n30.h gets SprRect through here) */

/* ------------------------------------------------------------------------ */
/* Main loop                                                                 */
/* ------------------------------------------------------------------------ */
extern u8 gsMainPacketBuf[];    /* 0x004ED700, main GS packet buffer (GsPacketBufInit in SysDataInit) */

/* ------------------------------------------------------------------------ */
/* Face portraits (ChrPortrait* 0x0040E928-0x0040EE18)                       */
/* ------------------------------------------------------------------------ */
/* GRXSYS face index table (sysGrxsysPac->portraitTable), relocated in place
 * by ChrPortraitTableInit. */
typedef struct ChrPortraitEntry {
    /* 0x0 */ u16 code;         /* portrait code (ChrPortraitSetByCode) */
    /* 0x2 */ u16 faceIndex;    /* SYS/FACE.DAT entry, 0xFFFF = none (ChrPortraitExists) */
    /* 0x4 */ u16 nameIndex;    /* into names[] (ChrPortraitGetName) */
    /* 0x6 */ u16 unk6;
} ChrPortraitEntry; /* size 0x8 */

typedef struct ChrPortraitIndex {
    /* 0x0 */ u32 count;        /* entries */
    /* 0x4 */ u32 nameCount;
    /* 0x8 */ s32 *names;       /* offsets, relocated to text pointers */
    /* 0xC */ ChrPortraitEntry *entries;
} ChrPortraitIndex; /* size 0x10 */

/* One face image slot (0x5B00, ChrPortraitAllocSlots): a FACE.DAT entry
 * decompressed in place, then its texture upload packet. */
typedef struct ChrPortrait {
    /* 0x0000 */ u8 tex[0x5980];       /* texture file (GphTexDataRelocate) */
    /* 0x5980 */ u8 texPacket[0x140];  /* GsTexLoadPacket; ChrPortraitGetTex returns its address */
    /* 0x5AC0 */ s32 cur;              /* entry loaded, -1 none (u32 in ChrPortraitSetByCode) */
    /* 0x5AC4 */ u8 pad5AC4[0x3C];
} ChrPortrait; /* size 0x5B00 */

typedef struct ChrPortraitSlots {
    /* 0x00 */ ChrPortrait *slots[8];
    /* 0x20 */ u32 *faceOffsets;    /* SYS/FACE.DAT offset table, relocated by ChrPortraitLoad */
    /* 0x24 */ u32 faceCount;       /* chrPortraitDataCount */
} ChrPortraitSlots; /* size 0x28 */

extern ChrPortraitSlots chrPortraitSlots;   /* 0x004ED590 */
extern s32 chrPortraitDataCount;            /* 0x004ED5B4 = chrPortraitSlots.faceCount */
extern ChrPortraitIndex *chrPortraitIndexTable; /* 0x004A948C */

/* ------------------------------------------------------------------------ */
/* 2D sprites (Spr* 0x0040EE18-0x00410538)                                   */
/* ------------------------------------------------------------------------ */
/* SprRect, SprRectListEntry and SprColorQuad are in sys.h. */

/* The sprite lists are gph.h batch quads, drawn by GphBatchDrawQuads
 * (quads) and GphBatchDrawSprites (sprites: corners 0 and 1); a list ends
 * at the first type 0. */
extern GphBatchQuad sprQuadList[];  /* 0x004A9D90, 32 quads + end marker (SprFlush) */
extern GphBatchQuad sprRectList[];  /* 0x004AA5E0, 32 sprites + end marker */
extern GphQuad3D sprQuad3DList[];   /* 0x004AAE30, 32 billboards + end marker */

/* ------------------------------------------------------------------------ */
/* Debug page strings shared by the particle debug pages (DbgPtcl*) and the  */
/* other edit pages. "\cN" switches the debug text colour.                  */
/* ------------------------------------------------------------------------ */
extern char dbgPtclLabelFmt[];          /* 0x004A15D8 "%s\n", a track that is off */
extern char dbgPtclTitleColorStr[];     /* 0x004A16B0 "\c5 " page title colour */
extern char dbgPtclColor0Str[];         /* 0x004A16B8 "\c0 " default colour */
extern char dbgPtclColor4Str[];         /* 0x004A16E0 "\c4 " highlight */
extern char dbgPosXFmt[];               /* 0x004A1718 "POS X %7.1f" */
extern char dbgPosYFmt[];               /* 0x004A1728 "Y %7.1f" */
extern char dbgPosZFmt[];               /* 0x004A1730 "Z %7.1f" */
extern char dbgRotXFmt[];               /* 0x004A1738 "ROT X 0x%.5X(%5.1f)" */
extern char dbgRotYFmt[];               /* 0x004A1750 "Y 0x%.5X(%5.1f)" */
extern char dbgRotZFmt[];               /* 0x004A1760 "Z 0x%.5X(%5.1f)" */
extern char dbgPtclOnStr[];             /* 0x004A1838 "ON" */
extern char dbgPtclOffStr[];            /* 0x004A1840 "OFF" */
extern char dbgPtclAlphaFmt[];          /* 0x004A1848 "ALPHA %2.1d" */
extern char dbgPtclDirGeneFmt[];        /* 0x004A1858 "DIR GENE (%s)" */
extern char dbgPtclFollowFmt[];         /* 0x004A1868 "FOLLOW (%s)" */
extern char dbgPtclUseAxFmt[];          /* 0x004A1878 "USE AX (%s)" */
extern char dbgPtclUseAyFmt[];          /* 0x004A1888 "USE AY (%s)" */
extern char dbgPtclUseAccFmt[];         /* 0x004A1898 "USE ACC (%s)" */
extern char dbgPtclAccStopFmt[];        /* 0x004A18A8 "ACC STOP (%s)" */
extern char dbgPtclAddModeFmt[];        /* 0x004A18B8 "ADD MODE (%s)" */
extern char dbgPtclSizeLinkFmt[];       /* 0x004A18C8 "SIZE (X %s Y)" */
extern char dbgPtclEqualStr[];          /* 0x004A18D8 "=" */
extern char dbgPtclNotEqualStr[];       /* 0x004A18E0 "!=" */
extern char dbgPtclRgbChangeFmt[];      /* 0x004A18E8 "RGB CHANGE (%s)" */
extern char dbgPtclRhFmt[];             /* 0x004A18F8 "RH (%s)" */
extern char dbgPtclRndStr[];            /* 0x004A1900 "RND" */
extern char dbgPtclRvFmt[];             /* 0x004A1908 "RV (%s)" */
extern char dbgPtclRXyzFmt[];           /* 0x004A1910 "R XYZ (%s)" */
extern char dbgPtclEditBoxFmt[];        /* 0x004A1970 "%s%s%s" */
extern char dbgPtclColor2Str[];         /* 0x004A1978 "\c2 " */
extern char dbgPtclEditBoxStr[];        /* 0x004A1980 "[   ]" */
extern char dbgPtclEditBoxBlinkStr[];   /* 0x004A1988 "<   >" */
extern char dbgPtclGenTitleFmt[];       /* 0x004A1B70 "Generator [%d] %s page %d/5%s" */
extern char dbgPtclEffeAFmt[];          /* 0x004A1BA8 "EFFE A %2.1d" */
extern char dbgPtclEffeBFmt[];          /* 0x004A1BB8 "B %2.1d" */
extern char dbgPtclEffeCFmt[];          /* 0x004A1BC0 "C %2.1d" */
extern char dbgPtclEffeDFmt[];          /* 0x004A1BC8 "D %2.1d" */
extern char dbgPtclAmbRFmt[];           /* 0x004A1BD0 "AMB R %5.1f" */
extern char dbgPtclAmbGFmt[];           /* 0x004A1BE0 "G %5.1f" */
extern char dbgPtclAmbBFmt[];           /* 0x004A1BE8 "B %5.1f" */
extern char dbgPtclAmbAFmt[];           /* 0x004A1BF0 "A %5.1f" */
extern char dbgPtclAxFmt[];             /* 0x004A1C38 "AX 0x%.5x(%5.1f)" */
extern char dbgPtclAxWidthFmt[];        /* 0x004A1C50 "W 0x%.5x(%5.1f)" */
extern char dbgPtclAyFmt[];             /* 0x004A1C60 "AY 0x%.5x(%5.1f)" */
extern char dbgPtclSpdFmt[];            /* 0x004A1C78 "SPD  %6.2f" */
extern char dbgPtclVal62fFmt[];         /* 0x004A1C88 "%6.2f" */
extern char dbgPtclAccFmt[];            /* 0x004A1C90 "ACC  %6.2f" */
extern char dbgPtclLifeFmt[];           /* 0x004A1CA0 "LIFE %4.1d" */
extern char dbgPtclVal4dFmt[];          /* 0x004A1CB0 "%4.1d" */
extern char dbgPtclProbFmt[];           /* 0x004A1CB8 "PROB %4.3f" */
extern char dbgPtclVal43fFmt[];         /* 0x004A1CC8 "%4.3f" */
extern char dbgPtclRadiusFmt[];         /* 0x004A1CD0 "R  %5.1f" */
extern char dbgPtclVal51fFmt[];         /* 0x004A1CE0 "%5.1f" */
extern char dbgPtclAngXFmt[];           /* 0x004A1CE8 "ANG X  0x%.5x(%5.1f)" */
extern char dbgPtclAngXWidthFmt[];      /* 0x004A1D00 "XW 0x%.5x(%5.1f)" */
extern char dbgPtclAngYFmt[];           /* 0x004A1D18 "Y  0x%.5x(%5.1f)" */
extern char dbgPtclAngYWidthFmt[];      /* 0x004A1D30 "YW 0x%.5x(%5.1f)" */
extern char dbgPtclOnceNumFmt[];        /* 0x004A1D48 "ONCE NUM %4.1d" */
extern char dbgPtclSx0Fmt[];            /* 0x004A1DB0 "SX 0    %5.1f" */
extern char dbgPtclRateStFmt[];         /* 0x004A1DC0 "RATE ST %5.1f" */
extern char dbgPtclC1fFmt[];            /* 0x004A1DD0 "C1 %5.1f" */
extern char dbgPtclC2fFmt[];            /* 0x004A1DE0 "C2 %5.1f" */
extern char dbgPtclEdfFmt[];            /* 0x004A1DF0 "ED %5.1f" */
extern char dbgPtclC1dFmt[];            /* 0x004A1E00 "C1 %4.1d" */
extern char dbgPtclC2dFmt[];            /* 0x004A1E10 "C2 %4.1d" */
extern char dbgPtclStC1Fmt[];           /* 0x004A1E20 "ST-C1 %3.1d" */
extern char dbgPtclC1C2Fmt[];           /* 0x004A1E30 "C1-C2 %3.1d" */
extern char dbgPtclC2EdFmt[];           /* 0x004A1E40 "C2-ED %3.1d" */
extern char dbgPtclSinXLFmt[];          /* 0x004A1E50 "SINX L %4.1d" */
extern char dbgPtclStdFmt[];            /* 0x004A1E60 "ST %4.1d" */
extern char dbgPtclRateFmt[];           /* 0x004A1E70 "RATE %4.2f" */
extern char dbgPtclSy0Fmt[];            /* 0x004A1E80 "SY 0    %5.1f" */
extern char dbgPtclSinYLFmt[];          /* 0x004A1E90 "SINY L %4.1d" */
extern char dbgPtclStRFmt[];            /* 0x004A1F30 "ST R %4.1d" */
extern char dbgPtclColGFmt[];           /* 0x004A1F40 "G %4.1d" */
extern char dbgPtclColBFmt[];           /* 0x004A1F48 "B %4.1d" */
extern char dbgPtclColAFmt[];           /* 0x004A1F50 "A %4.1d" */
extern char dbgPtclC1RFmt[];            /* 0x004A1F58 "C1 R %4.1d" */
extern char dbgPtclC2RFmt[];            /* 0x004A1F68 "C2 R %4.1d" */
extern char dbgPtclEdRFmt[];            /* 0x004A1F78 "ED R %4.1d" */
extern char dbgPtclRx0Fmt[];            /* 0x004A1FE8 "RX 0 0x%.5x(%5.1f)" */
extern char dbgPtclAngleValFmt[];       /* 0x004A2000 "0x%.5x(%5.1f)" */
extern char dbgPtclRy0Fmt[];            /* 0x004A2010 "RY 0 0x%.5x(%5.1f)" */
extern char dbgPtclRz0Fmt[];            /* 0x004A2028 "RZ 0 0x%.5x(%5.1f)" */
extern char dbgPtclRdxMdFmt[];          /* 0x004A2040 "RDX_MD %3.1d" */
extern char dbgPtclRdxStartFmt[];       /* 0x004A2050 "RDX S 0x%.5x(%5.1f)" */
extern char dbgPtclRdxEndFmt[];         /* 0x004A2068 "RDX E 0x%.5x(%5.1f)" */
extern char dbgPtclRdyMdFmt[];          /* 0x004A2080 "RDY_MD %3.1d" */
extern char dbgPtclRdyStartFmt[];       /* 0x004A2090 "RDY S 0x%.5x(%5.1f)" */
extern char dbgPtclRdyEndFmt[];         /* 0x004A20A8 "RDY E 0x%.5x(%5.1f)" */
extern char dbgPtclRdzMdFmt[];          /* 0x004A20C0 "RDZ_MD %3.1d" */
extern char dbgPtclRdzStartFmt[];       /* 0x004A20D0 "RDZ S 0x%.5x(%5.1f)" */
extern char dbgPtclRdzEndFmt[];         /* 0x004A20E8 "RDZ E 0x%.5x(%5.1f)" */

#endif
