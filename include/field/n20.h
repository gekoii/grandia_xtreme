/*
 * FIELD object move-command queue (0x005F7D98-0x005F8CD8), field character
 * list and party helpers (0x005F8CD8-0x005FC120), scenario message layout and
 * the character-select scene (0x005FC120-0x005FEE08), character turning
 * (0x005FEE08-0x005FFA58) and the mini-map (0x005FFA58-0x006018F0).
 *
 * Layouts were merged from the per-file views by byte offset.
 */
#ifndef FIELD_N20_H
#define FIELD_N20_H

#include "common.h"
#include "vu.h"
#include "gph.h"
#include "field/state.h"

/* ---------------------------------------------------------------- move commands */

/* FldMoveCmd payload of kind 0 (FldObjMoveTo/Velocity) and kind 3. */
typedef struct FldMoveCmdMove {
    /* 0x00 */ f32 pos[3];
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ f32 speed;          /* kind 3: first value */
    /* 0x14 */ f32 speed2;         /* kind 3: second value (FldMoveCmdGlobalAdd32) */
} FldMoveCmdMove; /* size 0x18 */

/* FldMoveCmd payload of kind 4 (FldMoveCmdGlobalAdd41/42). */
typedef struct FldMoveCmdK4 {
    /* 0x00 */ s32 a;
    /* 0x04 */ s32 b;
    /* 0x08 */ f32 c;
    /* 0x0C */ f32 d;
    /* 0x10 */ s16 e;
} FldMoveCmdK4; /* size 0x14 */

/*
 * One queued move command (FldMoveCmdAlloc returns &slot->cmd). Objects keep
 * a list at ChrObj.moveCmds (+0x64); the FldMoveCmdGlobalAdd* commands go to
 * fldMoveCmdGlobalList. The executor is 0x005F8828 (asm).
 */
typedef struct FldMoveCmd {
    /* 0x00 */ s16 id;             /* fldMoveCmdNextId at allocation */
    /* 0x02 */ s8 kind;            /* 0 object move, 1-4 global (FldMoveCmdGlobalAdd<kind><type>) */
    /* 0x03 */ s8 type;            /* kind 0: 2 move to, 3 velocity */
    /* 0x04 */ s8 state;           /* 0 new, 2 cancelled */
    /* 0x05 */ u8 flags;           /* kind 0: 0x10, 0x20 from the caller's flags 1/2 */
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ union {
        FldMoveCmdMove move;
        struct {                   /* kinds 1, 2 */
            s32 a;
            s32 b;
            s16 c;
            s16 d;
        } i;
        struct {                   /* kind 1 type 2 (FldMoveCmdGlobalAdd12) */
            s32 a;
            f32 b;
            s16 c;
            s16 d;
        } f;
        FldMoveCmdK4 k4;
    } u;
    /* 0x28 */ u8 pad28[8];
    /* 0x30 */ struct FldMoveCmd *next;
    /* 0x34 */ u8 pad34[0xC];
} FldMoveCmd; /* size 0x40 */

typedef struct FldMoveCmdSlot {
    /* 0x00 */ s32 used;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ FldMoveCmd cmd;
} FldMoveCmdSlot; /* size 0x50 */

typedef struct FldMoveCmdPool {
    /* 0x00 */ s32 count;          /* 0x100 */
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ FldMoveCmdSlot slots[1]; /* count entries (0x5010 bytes allocated) */
} FldMoveCmdPool;

/* ---------------------------------------------------------------- characters */

/* chrList: the 9 field characters by number (ChrGetByNo n = index + 1). */
typedef struct ChrList {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ ChrObj *chars[9];   /* ChrObj +0x2 holds the character id (ChrRegister) */
} ChrList; /* size 0x28 (ChrListClear) */

/* ---------------------------------------------------------------- message layout */

/* Text run for EvtMsgLayoutRun/EvtMsgLayoutChars (filled by EvtMsgTextUpdate). */
typedef struct EvtMsgText {
    /* 0x00 */ s32 layout;         /* TxtGlyphLayout slot 0/1: EvtMsgTextUpdate stores a
                                  * word, the layout functions read the low byte */
    /* 0x04 */ s32 w;
    /* 0x08 */ s32 h;
    /* 0x0C */ s32 x;
    /* 0x10 */ s32 y;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 *text;
} EvtMsgText; /* size 0x1C */

/* ---------------------------------------------------------------- character select */

/* EvtCharSelWork.state */
#define EVT_CHAR_SEL_OFF    0
#define EVT_CHAR_SEL_START  1
#define EVT_CHAR_SEL_CHOOSE 2
#define EVT_CHAR_SEL_SCRIPT 3
#define EVT_CHAR_SEL_END    4

/*
 * Character-select scene (EvtCharSelStart/Update, 0x005FD8E8-0x005FEE08).
 * Each FVECTOR triple is a VuBezier3 blend: value, from, to.
 */
typedef struct EvtCharSelWork {
    /* 0x000 */ u8 idleCount[5];   /* idle motions left before the next random one */
    /* 0x005 */ u8 flagIds[5];     /* system flag per character (0x33 + i) */
    /* 0x00A */ u16 state;         /* EVT_CHAR_SEL_*; EvtCharSelIsActive */
    /* 0x00C */ s16 mode;          /* EvtCharSelStart's first argument */
    /* 0x00E */ u8 padE[2];
    /* 0x010 */ u32 cur;           /* selected character */
    /* 0x014 */ u32 count;
    /* 0x018 */ s32 spin;          /* cursor spin angle */
    /* 0x01C */ s32 unk1C;
    /* 0x020 */ f32 lookT;
    /* 0x024 */ f32 eyeT;
    /* 0x028 */ f32 cursorT;
    /* 0x02C */ f32 bobT;
    /* 0x030 */ f32 spinSpeed;
    /* 0x034 */ u8 pad34[0xC];
    /* 0x040 */ FVECTOR look;      /* camera target (EvtCharSelCamSet) */
    /* 0x050 */ FVECTOR lookFrom;
    /* 0x060 */ FVECTOR lookTo;
    /* 0x070 */ FVECTOR eye;       /* camera position */
    /* 0x080 */ FVECTOR eyeFrom;
    /* 0x090 */ FVECTOR eyeTo;
    /* 0x0A0 */ FVECTOR bob;       /* cursor bob, x = height */
    /* 0x0B0 */ FVECTOR bobFrom;
    /* 0x0C0 */ FVECTOR bobTo;
    /* 0x0D0 */ FVECTOR eyeKey;    /* formation camera (evtCharSelCam2..5) */
    /* 0x0E0 */ FVECTOR cursorPos;
    /* 0x0F0 */ FVECTOR cursorFrom;
    /* 0x100 */ FVECTOR cursorTo;
    /* 0x110 */ ChrObj *chars[5];
    /* 0x124 */ GphModel *cursor;  /* map model 0xC9 (MapModelFindById) */
    /* 0x128 */ GphModel *cursor2; /* map model 0xC8 */
    /* 0x12C */ u8 pad12C[4];
    /* 0x130 */ FVECTOR lookKeys[5]; /* camera target per character */
} EvtCharSelWork; /* size 0x180 (EvtCharSelInit) */

/* ---------------------------------------------------------------- 3D/2D quads */

/* GphQuad3D (billboard quads, GphQuad3DSetCorners / GphBatchDrawQuads3D) is in gph.h. */

/* The mini-map frame/background quads are gph.h GphBatchQuads (type 1 frame
 * sprites (600D58), 5 flat colour, 0 end). */

/* ---------------------------------------------------------------- mini-map */

/* Floor texture header in a mini-map file (relocated by GphTexDataRelocate). */
typedef struct MapMiniTex {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ u8 *pixels;         /* MapMiniTexUpdate does address arithmetic on it as s32 */
    /* 0x20 */ u8 pad20[0x20];
    /* 0x40 */ u8 *clut;
} MapMiniTex;

typedef struct MapMiniFloor {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ u8 w;               /* 0x40 for auto maps */
    /* 0x03 */ u8 h;
    /* 0x04 */ f32 minY;           /* floor chosen above this height (MapMiniFloorSelect) */
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ u8 pad10[0x10];
} MapMiniFloor; /* size 0x20 */

typedef struct MapMiniHdr {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u32 floorCount;     /* up to 3 */
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ MapMiniTex *tex[4]; /* file offsets, relocated by MapMiniLoad */
    /* 0x20 */ MapMiniFloor floors[1];
} MapMiniHdr;

/* The mini-map model pack (GRXSYS +0x148) as MapMiniInit/MapMiniTexUpdate see it. */
typedef struct MapMiniPac {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ MapMiniTex *tex;    /* texture redirected to the floor buffers */
} MapMiniPac;

typedef struct MapMiniWork {
    /* 0x000 */ u8 showDelay;      /* counts to 60 before the map shows (MapMiniIsHidden) */
    /* 0x001 */ u8 pad1;
    /* 0x002 */ u8 floorCount;
    /* 0x003 */ u8 texFloor;       /* floor whose texture the model shows */
    /* 0x004 */ u8 floor;          /* MapMiniFloorSelect */
    /* 0x005 */ u8 pad5[3];
    /* 0x008 */ s32 model;         /* ChrModelCreate (a GphModel, s32 in all its uses) */
    /* 0x00C */ MapMiniHdr *hdr;
    /* 0x010 */ f32 zoomT;         /* zoom blend, 0..1 */
    /* 0x014 */ f32 iconScale;
    /* 0x018 */ f32 floorMinY[3];
    /* 0x024 */ u8 pad24[0xC];
    /* 0x030 */ FVECTOR zoomFrom;  /* camera pitch, distance, icon scale */
    /* 0x040 */ FVECTOR zoomTo;
    /* 0x050 */ u8 iconAlpha[0x40]; /* per tenant, 0x80 steady */
    /* 0x090 */ u8 iconSeen[0x40];
    /* 0x0D0 */ u8 *texBufs[3];    /* 0x8000 each, revealed part of each floor */
    /* 0x0DC */ u8 *texSrc[3];     /* full floor textures */
    /* 0x0E8 */ u8 *clut;          /* 0x40 bytes */
    /* 0x0EC */ u8 padEC[4];
    /* 0x0F0 */ GphBatchQuad quads[2]; /* frame sprites (600D58); 601138 draws quads[0] alone as the background */
    /* 0x170 */ s32 quadsEnd;
    /* 0x174 */ u8 pad174[0xFC];
    /* 0x270 */ GphQuad3D icons[64];
} MapMiniWork; /* size 0x1E70 (MapMiniInit) */

/* ---------------------------------------------------------------- globals */

extern FldMoveCmdPool *fldMoveCmdPool;
extern FldMoveCmd *fldMoveCmdGlobalList;
extern u16 fldMoveCmdNextId;
extern ChrList chrList;
extern u8 chrPartyOrderBackup[4];
extern u8 evtScnrHighIdLatchFlag;
extern EvtCharSelWork *evtCharSelWork;
extern MapMiniWork *mapMiniWork;
extern MapMiniHdr mapMiniAutoHdr;          /* one-floor header for auto maps */
extern MapMiniPac *mapMiniPac;

#endif /* FIELD_N20_H */
