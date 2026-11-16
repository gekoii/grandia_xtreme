#ifndef BATTLE_N34_H
#define BATTLE_N34_H

#include "common.h"

/*
 * BATTLE debug menu pages (0x006D9FE8-0x006DEC00): the DATA/ENV/SYS
 * sub-menus, the HIT DISP collision display, the CHAR WORK / CHAR PARAM /
 * PC SAVE / PAR TEST / IP STOP pages. The unit and party records they show
 * are in battle/unit.h and party.h; this header has the debug state shared
 * by several of the page files.
 */

/* ---------------------------------------------------------------- page state */

/* Unit index (0-15) shown as "CHAR (%d)" by CHAR WORK, CHAR PARAM and PAR TEST. */
extern s32 dbgBtlCharNo;                /* 0x006DF4F0 */

/* Shared strings of the page headers and value columns (rodata). */
extern char dbgBtlColor1Str[];          /* 0x006E2B98 "\c1 " title colour */
extern char dbgBtlColor0Str[];          /* 0x006E2BA0 "\c0 " colour reset */
extern char dbgBtlCharNoFmt[];          /* 0x006E2C20 "CHAR (%d)" */
extern char dbgBtlIndexValueFmt[];      /* 0x006E3020 "%d %d" */
extern char dbgBtlIntFmt[];             /* 0x006E30A0 "%d" */
extern char dbgBtlFloatFmt[];           /* 0x006E34D8 "%.2f" */

/* ---------------------------------------------------------------- HIT DISP */

/* dbgBtlHitDispFlags (HIT DISP page, DbgBtlHitDispPage 0x006DADE8) */
#define DBG_BTL_HIT_FIELD  0x1          /* "FIELD": arena bounds */
#define DBG_BTL_HIT_CHAR   0x2          /* "CHAR": unit collision cylinders */
#define DBG_BTL_HIT_ATTACK 0x4          /* "ATACK": attack marks */

extern u8 dbgBtlHitDispFlags;           /* 0x006F60A0; s8 in DbgBtlHitDispInit (store only) */

/*
 * Attack mark (DbgBtlHitMarkAdd 0x006DB4E8), a ring of 16. Drawn as a
 * cylinder at (x, 0, z) with the given radius while alpha is non-zero
 * (DbgBtlHitDispDraw 0x006DAFE0); DbgBtlHitMarkFade (0x006DAFA8) takes 4
 * off alpha every frame.
 */
typedef struct DbgBtlHitMark {
    /* 0x0 */ u8 alpha;                 /* 0x80 when added, also the colour's alpha */
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ f32 x;
    /* 0x8 */ f32 z;
    /* 0xC */ f32 radius;
} DbgBtlHitMark; /* size 0x10 */

extern s8 dbgBtlHitMarkNext;            /* 0x006F60A1 next ring slot */
extern DbgBtlHitMark dbgBtlHitMarks[16]; /* 0x006F60A8 */

#endif /* BATTLE_N34_H */
