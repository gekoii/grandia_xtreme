#ifndef FIELD_N14_H
#define FIELD_N14_H

/*
 * FIELD 0x005C1190-0x005C95E8: tenant (NPC) AI tracks and wander areas,
 * trap damage numbers and the HP bar, position trails, paths and splines,
 * object motion tables, message-window glyph layout and queues.
 * Layouts come from the files' own views (docs/notes/names_observations.md, n14).
 */

#include "common.h"
#include "vu.h"
#include "field/state.h"

/* ---------------------------------------------------------------- 2D sprites */

/* Sprites of GphBatchDrawSprites lists are gph.h GphBatchQuads (corners 0
 * and 1 carry the texture and screen rectangles; a list ends at type 0). */

/* ---------------------------------------------------------------- tenant AI tracks */

/* One AI op: chrTenantOpTable[op](track, obj, arg1, arg2). */
typedef struct ChrTenantOp {
    /* 0x0 */ s16 op;              /* 0..0x1D */
    /* 0x2 */ s16 arg1;
    /* 0x4 */ s16 arg2;
} ChrTenantOp; /* size 0x6 */

typedef struct ChrTenantTrack {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;           /* cleared by ChrTenantTrackLoad */
    /* 0x04 */ s16 unk4;           /* cleared by ChrTenantTrackLoad */
    /* 0x06 */ ChrTenantOp ops[32]; /* the presets end a track with op 1 or op 0x1D */
} ChrTenantTrack; /* size 0xC6 */

typedef struct ChrTenantTrackPool {
    /* 0x0 */ s32 count;           /* clamped to 64 */
    /* 0x4 */ ChrTenantTrack *tracks;
} ChrTenantTrackPool; /* size 0x8 */

typedef s32 (*ChrTenantOpFunc)(ChrTenantTrack *track, ChrObj *obj, s16 arg1, s16 arg2);

/* ---------------------------------------------------------------- tenant sense record */

/* ChrObj.sense: the tenant's ChrEnemyWork (field/state.h). */
#define CHR_SENSE(obj) ((obj)->sense)

/* ---------------------------------------------------------------- wander areas */

/* Completes the tag that field/state.h's ChrObj.areas[] points to. */
typedef struct ChrTenantArea {
    /* 0x00 */ u16 id;
    /* 0x02 */ u8 kind;            /* 1/2 can be picked by ChrTenantAreaPickRandom, +10 once picked */
    /* 0x03 */ u8 pointCount;      /* points[0..pointCount-1] in use (op 3 picks 1..pointCount) */
    /* 0x04 */ f32 x0;             /* rectangle, either corner order */
    /* 0x08 */ f32 z0;
    /* 0x0C */ f32 x1;
    /* 0x10 */ f32 z1;
    /* 0x14 */ VuVec3 centre;
    /* 0x20 */ VuVec3 points[6];
} ChrTenantArea; /* size 0x68 */

/* An area rectangle with sorted corners (the debug draws and area checks). */
typedef struct ChrTenantRect {
    /* 0x0 */ f32 x0;
    /* 0x4 */ f32 z0;
    /* 0x8 */ f32 x1;
    /* 0xC */ f32 z1;
} ChrTenantRect; /* size 0x10 */

typedef struct ChrTenantAreaTable {
    /* 0x0 */ s32 count;           /* clamped to 0x9D */
    /* 0x4 */ ChrTenantArea *areas;
} ChrTenantAreaTable; /* size 0x8 */

/* ---------------------------------------------------------------- position trail */

/* Completes the tag of ChrObj.trail (+0x6C): a ring of the last 60 positions. */
typedef struct ChrTrail {
    /* 0x000 */ FVECTOR pos[60];   /* w = distance from the previous entry */
    /* 0x3C0 */ s32 tag[60];
    /* 0x4B0 */ s32 head;
} ChrTrail; /* size 0x4B4 */

/* ChrMotTable (ChrObj.motTable) is in field/state.h. */

/* ---------------------------------------------------------------- trap damage / HP bar */

typedef struct FldDmgPopWork {
    /* 0x0000 */ GphBatchQuad bars[4];      /* HP bar sprites (texture 0xE9) */
    /* 0x0100 */ GphBatchQuad digits[257];  /* damage number sprites (texture 0xEB) */
    /* 0x4140 */ FVECTOR pos[32];
    /* 0x4340 */ f32 t[32];                   /* 0..1 rise, 1..2 fade (s32 0 in 5C57C8) */
    /* 0x43C0 */ u16 value[32];               /* s16 in 5C57C8 */
    /* 0x4400 */ u8 unk4400[32];
    /* 0x4420 */ u8 active[32];
    /* 0x4440 */ u8 count;                    /* sprites drawn this frame, 0 = skip FldDmgPopDraw */
    /* 0x4441 */ u8 next;                     /* next slot */
    /* 0x4442 */ u8 pad4442[0xE];
} FldDmgPopWork; /* size 0x4450 */

/* ---------------------------------------------------------------- paths */

typedef struct MapPathSmall {
    /* 0x000 */ f32 x[64];
    /* 0x100 */ f32 y[64];
    /* 0x200 */ f32 z[64];
} MapPathSmall; /* size 0x300 */

typedef struct MapPathBig {
    /* 0x0000 */ f32 x[0x220];
    /* 0x0880 */ f32 y[0x220];
    /* 0x1100 */ f32 z[0x220];
} MapPathBig; /* size 0x1980 */

typedef struct MapPathWork {
    /* 0x0000 */ f32 x[20];        /* control points (MapPathBuild) */
    /* 0x0050 */ f32 y[20];
    /* 0x00A0 */ f32 z[20];
    /* 0x00F0 */ f32 e[20];        /* event */
    /* 0x0140 */ f32 c[20];        /* curve type: 0 points, 1 linear, else spline */
    /* 0x0190 */ f32 px[0x220];    /* tessellated points (MapPathTessellate) */
    /* 0x0A10 */ f32 py[0x220];
    /* 0x1290 */ f32 pz[0x220];
    /* 0x1B10 */ MapPathSmall small[8];
    /* 0x3310 */ MapPathBig big[8];
    /* 0xFF10 */ s16 smallUsed[8]; /* point counts, 0 = free */
    /* 0xFF20 */ s16 bigUsed[8];
} MapPathWork; /* size 0xFF30 */

/* fldPathNodeData entry */
typedef struct MapPathNode {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ u8 type;            /* curve type */
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 event;
} MapPathNode; /* size 0x10 */

/* ---------------------------------------------------------------- message text glyphs */

/* txtGlyphLayout: 6 groups (0-2 system font images, 3-5 field message windows). */
typedef struct TxtGlyphLayout {
    /* 0x0000 */ s32 count;        /* characters; two sprites (shadow, glyph) each */
    /* 0x0004 */ s32 x;
    /* 0x0008 */ s32 y;
    /* 0x000C */ s32 padC;
    /* 0x0010 */ GphBatchQuad sprites[0x200];
} TxtGlyphLayout; /* size 0x8010 */

/* txtGlyphQueue: 6 queues of glyphs to draw into the font images (TxtGlyphQueueFlush). */
typedef struct TxtGlyphQueue {
    /* 0x000 */ u8 clearReq;       /* TxtGlyphQueueRequestClear: clear the image first */
    /* 0x001 */ u8 pad1[3];
    /* 0x004 */ s32 count;
    /* 0x008 */ u8 col[0x100];
    /* 0x108 */ u8 row[0x100];
    /* 0x208 */ u16 glyph[0x100];
} TxtGlyphQueue; /* size 0x408 */

/* ---------------------------------------------------------------- globals */

extern ChrTenantTrackPool *chrTenantTrackPool;  /* 0x00676700 */
extern ChrTenantAreaTable *chrTenantAreaTable;  /* 0x00666780 */
extern FldDmgPopWork *fldDmgPopWork;            /* 0x00676718 */
extern MapPathWork *mapPathWork;                /* 0x006763E0 */
extern MapPathNode *fldPathNodeData;            /* 0x006763F0 */
extern s32 mapPathCtrlCount;                    /* 0x00676720 */
extern s32 mapPathPointCount;                   /* 0x00676724 */
extern f32 *mapSplineSecondDeriv[3];            /* 0x00676728, 0x54 bytes each */
extern f32 *mapSplineTriUpper;                  /* 0x00676734 */
extern f32 *mapSplineTriDiag;                   /* 0x00676738 */
extern f32 *mapSplineStepLengths;               /* 0x0067673C */
extern TxtGlyphLayout *txtGlyphLayout;          /* 0x00676740 */
extern TxtGlyphQueue *txtGlyphQueue;            /* 0x00676744 */
extern u8 txtMsgWinOpened[4];                   /* 0x006762C0 */

#endif /* FIELD_N14_H */
