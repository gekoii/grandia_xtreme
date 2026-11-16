#ifndef GPH_H
#define GPH_H

/* Engine graphics of the main executable: models, parts, GA objects
 * (meshes), draw lists, lights, views, textures, 2D anim sets, the screen
 * shatter transition and the frame-buffer effects. Field names come from
 * docs/notes/names_observations.md (n03, n04, n05, n06, n07, n01), the symbol
 * files' comments, the debug pages and the code. A comment "X in 4xxxxx"
 * records a file whose local view used a different type at that offset. */

#include "common.h"

#include "vu.h"

/* ------------------------------------------------------------------ motion */

/* Motion data (model motion pack entry), pointed to by GphModel.motion[]. */
typedef struct GphMotion {
    /* 0x00 */ s32 unk0;        /* copied to cursor +0x14/+0x18 (track data) */
    /* 0x04 */ u16 frameCount;  /* ChrModelMotionUpdate: last frame = frameCount - 1 */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ u16 flags;       /* 0x30 vertex tracks (GphModelMorph), 0xFF0 material tracks */
    /* 0x0A */ u16 unkA;        /* low 4 bits -> cursor +0xC */
    /* 0x0C */ u16 partCount;   /* compared with GphModel.partCount (5C7D08, 7006E8) */
    /* 0x0E */ u16 playFlags;   /* bit 0 loop (436F60 tests & 1) */
} GphMotion; /* size >= 0x10 */

/* Motion cursor (scratchpad 0x700028C0/0x700028E0), GphMotCursorInit 434F78. */
typedef struct GphMotCursor {
    /* 0x00 */ f32 frame;
    /* 0x04 */ u16 flags;       /* = motion->flags */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ u16 frameCount;  /* = motion->frameCount */
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ s16 unkC;
    /* 0x0E */ u16 part;        /* advanced by GphMotCursorNextPart 434FB8 (s16 in 434F78) */
    /* 0x10 */ s16 unk10;
    /* 0x12 */ u8 pad12[2];
    /* 0x14 */ s32 *track;      /* 434FE8.. read it as {u16, u16, s32} records; 434FB8 reads the
                                 * part's u16 key count through a u16 * */
    /* 0x18 */ s32 *trackStart;
    /* 0x1C */ GphMotion *motion;
} GphMotCursor; /* size = 0x20 */

/* One motion slot of a model (+0x80 and +0x88). */
typedef struct GphMotSlot {
    /* 0x00 */ GphMotion *motion;
    /* 0x04 */ f32 frame;
} GphMotSlot; /* size = 0x8 */

/* -------------------------------------------------------- GA object / mesh */

/* GA object = mesh (0x80). One per mesh of a part; GphDrawListRender copies
 * all 0x80 bytes to scratchpad 0x70003800 and that copy is the "mesh header"
 * the VU1 render reads (observations n04 mesh header: byte 0 tri mode/type/billboard 0x20,
 * byte 2 clip flags, +0x3C light block, +0x44 tri count, +0x48 vertex count,
 * +0x50 index list, +0x58 matrix, +0x60 verts, +0x64 normals, +0x68 UVs,
 * +0x6C colours) - GphMesh is the same layout. */
typedef struct GphGaObject {
    /* 0x00 */ u8 mode;         /* tri mode/type, billboard 0x20 */
    /* 0x01 */ u8 flags;        /* bit 0 light alt (GphModelPartDraw), deformers test it */
    /* 0x02 */ u8 clip;         /* clip result / layer (0xFF = off, 43BAE0). s8 in 433650 and
                                 * 439FF8 (signed clip result); 43A498 reads it into a s16 layer */
    /* 0x03 */ u8 unk3;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 texId;       /* 438378 clears it; 423060 prints it as the texture id */
    /* 0x0C */ u8 *vtxPkt;      /* vertex packet the deformers/morph write: Vec4 * (4386A0),
                                 * u32 (439BF0), s32 (437F10, 438570), FVECTOR * (435A40) */
    /* 0x10 */ FVECTOR center;  /* bounding sphere (439328) */
    /* 0x20 */ f32 radius;
    /* 0x24 */ void *deformBuf0;    /* deformer output: positions (4380B8/438890), 16 bytes per vertex (437F10) */
    /* 0x28 */ f32 unk28;           /* 2000.0 (PtclInit 44A1F8) */
    /* 0x2C */ void *deformBuf1;    /* deformer output: normals (4380B8/438890) or UVs (439BF0), 8 bytes per vertex */
    /* 0x30 */ f32 unk30;       /* 434280: != 0 means translucent; ChrModelSetGaObjParam30 */
    /* 0x34 */ void *envUv;     /* env-map UV packet (43BAE0/43BC10); s32 in 437F10 */
    /* 0x38 */ f32 *ambient;    /* = &part->ambient (GphModelPartDraw) */
    /* 0x3C */ s32 lightBlock;  /* = *(s32 *)0x70002704 */
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 triCount;
    /* 0x48 */ s32 vtxCount;    /* u32 in 4386A0/438890 */
    /* 0x4C */ u16 *skin;       /* skin weights, non-NULL = skinned (u16 * in 433C18; s32 in
                                 * 433B10/437EA8/438890) */
    /* 0x50 */ void *indices;
    /* 0x54 */ s32 gsPkt;       /* part GS register packet (GphModelUpdateGsRegs), cleared by the draw */
    /* 0x58 */ s32 mtx;         /* matrix the draw uses (GphModelPartDraw arg); void * in 451A20 */
    /* 0x5C */ void *mtx2;      /* set to the matrix too (PtclEmitterDraw 451A20) */
    /* 0x60 */ FVECTOR *verts;  /* original vertices (u32 in 433C18) */
    /* 0x64 */ void *normals;   /* original normals, 4.12 fixed (u32 in 433C18, u8 * in 43BF40) */
    /* 0x68 */ void *uvs;       /* u32 * in 439BF0/43B310, u8 * in 43BF40 */
    /* 0x6C */ void *colors;
    /* 0x70 */ f32 unk70;       /* 1000.0 (43ADC8) */
    /* 0x74 */ f32 unk74;
    /* 0x78 */ f32 uOffset;     /* part UV scroll * 1/4096 (GphModelPartDraw) */
    /* 0x7C */ f32 vOffset;
} GphGaObject; /* size = 0x80 */

typedef GphGaObject GphMesh;

/* ------------------------------------------------------------------ part */

typedef struct GphPart {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 id;          /* ChrModelFindPart */
    /* 0x08 */ u16 flags;       /* 8 hidden (DISP OFF), 0x100 matrix override (motion mode 3) */
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 motionSlot;   /* split motion: 0 = motion A, else B */
    /* 0x0C */ f32 *mtx;        /* u32 in 43B7F0.. */
    /* 0x10 */ FVECTOR pos;     /* T/R/S block for VuMtxApplyTRS */
    /* 0x20 */ IVECTOR rot;     /* 16-bit angles (FIELD sets +0x24) */
    /* 0x30 */ FVECTOR scale;
    /* 0x40 */ GphGaObject *gaObjs;
    /* 0x44 */ GphGaObject *gaObjsCopy; /* per-frame copies (skinning, GphModelSkin) */
    /* 0x48 */ u16 child;
    /* 0x4A */ u16 next;        /* next sibling */
    /* 0x4C */ u16 groupPartCount; /* parts from this one on in its group (BATTLE 6B3730..) */
    /* 0x4E */ u8 pad4E[2];
    /* 0x50 */ u8 drawFlags;    /* bit 0 skinned / use gaObjsCopy */
    /* 0x51 */ u8 drawn;        /* set by GphModelPartDraw */
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u16 attr;        /* attributes: 1 lighting modes, 0x30, 0x80 ENVMAP ... (ChrPartSetAttr) */
    /* 0x56 */ u16 gsFlags;     /* GS render flags (GphPartBuildGsRegs), bits 11-12 ALPHA BLEND */
    /* 0x58 */ u8 alphaFix;     /* ALPHA FIX / alpha test ref */
    /* 0x59 */ u8 unk59;
    /* 0x5A */ u8 unk5A;
    /* 0x5B */ u8 prio;         /* PARTS PRIO, draw list layer */
    /* 0x5C */ u8 effect;       /* 5DF1A8 (FIELD) */
    /* 0x5D */ u8 pad5D;
    /* 0x5E */ u16 gaObjCount;
    /* 0x60 */ s16 uvScroll[4]; /* u, ?, v, ? (u16 in 436CF0/4371F8) */
    /* 0x68 */ s32 gsPkt;       /* GS register packet address (u32 in 434280) */
    /* 0x6C */ void *skinBuf;   /* 433C18 */
    /* 0x70 */ FVECTOR color;   /* surface RGBA, alpha +0x7C (scalar f32s in 43B4F8.., 436058) */
    /* 0x80 */ FVECTOR ambient; /* light block colour + surface colour (GphModelPartDraw) */
    /* 0x90 */ FVECTOR cullSphere;
} GphPart; /* size = 0xA0 */

/* ----------------------------------------------------------------- model */

typedef struct GphModel {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 objId;       /* object id (FIELD model table ids: 200+, 1000+); s32 in five
                                 * files, s16 in 5C0EE0 (cast) */
    /* 0x08 */ u16 flags;       /* 8 hidden (DISP OFF), 0x80 cull every part, 0x200 root rotation
                                 * onto part 1, 0x4000/0x8000 euler order, low 3 bits TRS off */
    /* 0x0A */ s8 cull;         /* sphere cull result, < 0 = outside */
    /* 0x0B */ u8 unkB;
    /* 0x0C */ f32 *mtx;
    /* 0x10 */ FVECTOR pos;
    /* 0x20 */ IVECTOR rot;     /* 16-bit angles; FVECTOR in 4344D8 (copied as a qword, y zeroed as 0.0f) */
    /* 0x30 */ FVECTOR scale;
    /* 0x40 */ u16 partCount;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ GphPart *parts;  /* s32 in 433310/434760 */
    /* 0x48 */ f32 *restMtx;
    /* 0x4C */ FMATRIX *viewMtx; /* nParts view*part matrices from vuMtxPool (43BAE0 0x40-byte work) */
    /* 0x50 */ f32 cullScale;   /* GphModelCalcAndDraw passes it to GphModelCalcCull */
    /* 0x54 */ u32 pktSize;     /* packet bytes per frame (s32 in 433310/43A0F8 u32) */
    /* 0x58 */ GphGaObject *gaObjs; /* flat list of every part's GA objects */
    /* 0x5C */ u16 gaObjCount;
    /* 0x5E */ u8 pad5E[2];
    /* 0x60 */ FVECTOR cullSphere;
    /* 0x70 */ s32 motionMode;  /* 0 none, single, split, blend, 3 override (GphModelCalcParts) */
    /* 0x74 */ f32 blend;       /* motion blend weight */
    /* 0x78 */ u8 pad78[8];
    /* 0x80 */ GphMotSlot motion[2]; /* +0x80/+0x84 main, +0x88/+0x8C sub (ChrModelSetMotion/SubMotion) */
} GphModel; /* size = 0x90 */

/* Motion state of a model, GphModel +0x70..+0x8F. GphModel lays these out as
 * separate GphModel fields (motionMode, blend, motion[2]), but the code
 * addresses them through a pointer to +0x70 (GphModelDrawMtx 433310,
 * GphModelCalcParts 434760, 436F60, 4381D8), which only matches with a
 * nested struct: those files take (GphModelMotion *)&model->motionMode. */
typedef struct GphModelMotion {
    /* 0x00 */ s32 mode;        /* GphModel.motionMode: 0 single, 1 split, 2 blend, 3 override */
    /* 0x04 */ f32 blend;       /* GphModel.blend */
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ GphMotSlot slot[2]; /* GphModel.motion: main, sub */
} GphModelMotion; /* size = 0x20 */

/* ------------------------------------------------------------- draw list */

/* Draw list entry (0x10), GphDrawListAddGaObj 43A498 / GphDrawListRender 436740. */
typedef struct GphDrawItem {
    /* 0x00 */ u8 texId;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 layer;        /* copy of gaObj->clip; 43A498 writes it as s16 h2 (with unk3) */
    /* 0x03 */ u8 unk3;
    /* 0x04 */ GphGaObject *gaObj;
    /* 0x08 */ s32 mtx;         /* matrix (void * work in 43BC10) */
    /* 0x0C */ s32 unkC;
} GphDrawItem; /* size = 0x10 */

typedef struct GphDrawList {
    /* 0x00 */ u32 count;
    /* 0x04 */ u32 capacity;
    /* 0x08 */ u32 layerStart[16]; /* from the key's top 4 bits (GphDrawListSort/Index) */
    /* 0x48 */ u64 *keys;       /* sort keys, low 32 bits point at the item */
    /* 0x4C */ GphDrawItem *items;
    /* 0x50 */ u16 *envMapList; /* {count, item indices...} (43BC10) */
} GphDrawList; /* size = 0x54 */

/* ---------------------------------------------------- render context */

/* Scratchpad render context at 0x70002700: +0 current camera
 * (CamSetCurrent 431288), +4 current light VU block, +8 current draw list
 * (n04.h gphDrawListCur), then counters the draws add to (GphMeshDraw*
 * leave their output counts at 0x700000E0/E4), cleared each frame by
 * GphClearRenderStats 430D10; +0x40 light state reset by GphResetSprLight
 * 430CD8. 0x70002740 holds a colour qword (4321C0 copies a light's colour
 * there). */
typedef struct GphRenderCtx {
    /* 0x00 */ struct Camera *camera; /* camera.h; CamSetCurrent */
    /* 0x04 */ s32 lightBlock;
    /* 0x08 */ GphDrawList *drawList;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s32 modelCount;  /* GphModelPartsDraw 433650: one per model */
    /* 0x14 */ s32 unk14;       /* += *0x700000E0 after a mesh draw */
    /* 0x18 */ s32 unk18;       /* += *0x700000E4 after a mesh draw */
    /* 0x1C */ s32 unk1C;       /* meshes whose 0x700000E0 count was 0 */
    /* 0x20 */ s32 gaObjCount;  /* GA objects seen (433650, 439FF8) */
    /* 0x24 */ s32 gaObjDrawn;  /* GA objects added to the draw list */
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 texUploads;  /* GphTexBind 43F8A0, GphEnvMapSendTex 43BCD8 */
    /* 0x30 */ s32 texUploadSize; /* += texture w * h */
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ f32 unk4C;       /* 128.0 */
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ f32 unk5C;
} GphRenderCtx; /* size >= 0x60 */

#define gphRenderCtx ((GphRenderCtx *)0x70002700)

/* ------------------------------------------------------ GS A+D packets */

/* One A+D register write: data, then the register address. */
typedef struct GsAdReg {
    /* 0x00 */ u64 data;
    /* 0x08 */ u64 addr;
} GsAdReg; /* size = 0x10 */

/* DMA/VIF/GIF packet of A+D register writes, laid out by GsAdPktInit 43B9E0
 * (DMA tag, VIF NOP x3 + DIRECT, GIF tag with A+D, then the registers) and
 * sent with GsGifPacketSend. */
typedef struct GsAdPkt {
    /* 0x00 */ u64 dmaTag;      /* cnt: qword count */
    /* 0x08 */ u64 dmaTagHi;
    /* 0x10 */ s32 vif[3];      /* NOP */
    /* 0x1C */ s32 direct;      /* VIF DIRECT, qword count */
    /* 0x20 */ u64 gifTag;
    /* 0x28 */ u64 gifRegs;     /* 0xE = A+D */
    /* 0x30 */ GsAdReg reg[1];  /* as many as the packet holds (GsAdPktInit's count) */
} GsAdPkt; /* size = 0x30 + 0x10 per register */

/* ----------------------------------------------------- display env */

/* sceGsDispEnv as the engine keeps it (gsDispEnv, 0x0051B100; GsVsyncFlip
 * 45EA88 writes the flipped frame address into dispfb). */
typedef struct GsDispEnv {
    /* 0x00 */ u64 pmode;
    /* 0x08 */ u64 smode2;
    /* 0x10 */ u64 dispfb;
    /* 0x18 */ u64 display;
    /* 0x20 */ u64 bgcolor;
} GsDispEnv; /* size 0x28 */

extern GsDispEnv gsDispEnv;     /* 0x0051B100 (0x40 reserved) */

/* GS frame buffer addresses (GsVsyncFlip 45EA88 flips draw/display,
 * GphFbSwapDrawZBuf 4421E0 swaps draw/Z so the effects can read the last
 * frame as a texture). u32: 43A370, 43A5D8, 456860 shift them right (srl),
 * 45EA88 widens one to a u64 (zero-extension). */
extern u32 gsDispFrameAddr;     /* 0x0049B6D0 */
extern u32 gsDrawFrameAddr;     /* 0x0049B6D4 */
extern u32 gsZBufAddr;          /* 0x0049B6D8 */

/* ----------------------------------------------------------------- light */

typedef struct GphLight {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s32 id;          /* GphLightFindById */
    /* 0x08 */ s32 type;        /* 0 directional, 1 point */
    /* 0x0C */ s32 enable;
    /* 0x10 */ FVECTOR dir;     /* direction or position (GphLightInit writes it as s32/f32 scalars) */
    /* 0x20 */ FVECTOR color;
    /* 0x30 */ f32 range;
    /* 0x34 */ f32 intensity;
} GphLight; /* size = 0x40 (0x38 used; the list's light array has a 0x40 stride) */

/* VU light block (0x190), GphLightVuBlockReset 432018, filled by GphLightListUpdate. */
typedef struct GphLightVuBlock {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ u32 vifCode;    /* 0x6C100180 */
    /* 0x010 */ FVECTOR unk10[8];
    /* 0x090 */ FVECTOR dirColor[4];
    /* 0x0D0 */ FVECTOR pointColor[4]; /* colour * intensity */
    /* 0x110 */ FVECTOR dirVec[4];     /* w = intensity */
    /* 0x150 */ FVECTOR pointPos[4];   /* w = 1 / range */
} GphLightVuBlock; /* size = 0x190 */

typedef struct GphLightList {
    /* 0x000 */ u32 count;
    /* 0x004 */ u32 capacity;
    /* 0x008 */ u8 pad8[8];
    /* 0x010 */ GphLightVuBlock block; /* scratchpad 0x70002704 points here */
    /* 0x1A0 */ GphLight *lights[1];   /* count entries, then the 32-aligned light array */
} GphLightList;


/* ------------------------------------------------------------------ views */

/* Viewport (gphViews, 8). GphViewSetup 42F628 = SetArea + SetClip + SetFov. */
typedef struct GphView {
    /* 0x000 */ u8 drawEnv[0x80];  /* GS draw environment packet (GsDrawEnvPacketSet) */
    /* 0x080 */ u8 fogPkt[0x50];   /* FOGCOL packet (GsFogPacketSet); 42FA40 views 0x80 bytes */
    /* 0x0D0 */ f32 x;             /* area (GphViewSetArea) */
    /* 0x0D4 */ f32 y;
    /* 0x0D8 */ f32 w;
    /* 0x0DC */ f32 h;
    /* 0x0E0 */ f32 centerX;
    /* 0x0E4 */ f32 centerY;
    /* 0x0E8 */ f32 magH;          /* SCREEN MAG H */
    /* 0x0EC */ f32 magV;          /* SCREEN MAG V */
    /* 0x0F0 */ f32 projDist;      /* 320 / tan(fov / 2) */
    /* 0x0F4 */ f32 clipNear;      /* GphViewSetClip */
    /* 0x0F8 */ f32 clipFar;
    /* 0x0FC */ f32 unkFC;         /* 16777215.0 at init */
    /* 0x100 */ f32 fogParam[3];   /* GphViewSetFog ranges (10000, 255.999, 0 at init) */
    /* 0x10C */ f32 fov;           /* degrees */
    /* 0x110 */ u8 fogR;
    /* 0x111 */ u8 fogG;
    /* 0x112 */ u8 fogB;
    /* 0x113 */ u8 pad113[0x2D];
} GphView; /* size = 0x140 */

/* --------------------------------------------------------------- textures */

/* Texture file header (TXD / PAC textures; GphTexDataRelocate 43F360 turns
 * its 15 offsets into pointers). Also the frame-buffer texture descriptors. */
typedef struct GphTex {
    /* 0x00 */ u32 w;
    /* 0x04 */ u32 h;
    /* 0x08 */ u32 tw;             /* log2 width (TEX0 TW) */
    /* 0x0C */ u32 th;
    /* 0x10 */ u32 psm;
    /* 0x14 */ u32 unk14;          /* a word (GsTexInfoInit 430450); 430570 reads the low half */
    /* 0x18 */ u32 size;           /* bytes */
    /* 0x1C */ void *data[7];      /* u32 offsets before relocation (43F360) */
    /* 0x38 */ s32 unk38;          /* 1 (GsTexInfoInit) */
    /* 0x3C */ s32 unk3C;          /* 0x40 */
    /* 0x40 */ void *unk40[8];     /* u32 offsets before relocation */
    /* 0x60 */ s64 id;             /* own texture id (GphTexRegisterOwnId); 40BFA8 stores a u64;
                                    * 43F578 reads the low byte through a u8 * */
    /* 0x68 */ u8 pad68[4];
    /* 0x6C */ s32 uploadMode;     /* 3 or 0, passed to 456B80 (GphTexSetMode) */
    /* 0x70 */ u8 pad70[0xC];
    /* 0x7C */ s32 texId;          /* BATTLE 6B3A68/6B3B48 */
} GphTex; /* size = 0x80 (gphFbTexInfo and gphFbUploadTexInfo are 0x80 apart) */

/* A GS texture object as filled by GsTexLoadPacket and sent by GsTexUpload /
 * GsTexPacketSend (0x140 bytes; TxtFontImage.texPacket is one too). */
typedef struct GsTexPacket {
    /* 0x000 */ u8 data[0x140];
} GsTexPacket; /* size 0x140 */

/* Texture table slot (0x2C0). */
typedef struct GphTexSlot {
    /* 0x000 */ u8 flag;           /* 0xFF = free */
    /* 0x001 */ u8 savedMode;      /* uploadMode at registration (GphTexRestoreMode) */
    /* 0x002 */ u8 pad2[6];
    /* 0x008 */ GphTex *src;       /* 43F838/43F9A0 compare/return it as an s32 (casts) */
    /* 0x00C */ u8 padC[0x34];
    /* 0x040 */ u8 pkt[0x280];     /* upload/bind packet (456B80) */
} GphTexSlot; /* size = 0x2C0 */

/* Texture table (gphTexTable; GphTexTableSize/Init 43F508/43F518). Ids index
 * slots[] directly (id 0 is treated as 1; 43F978 GphTexGetSource). */
typedef struct GphTexTable {
    /* 0x00 */ s32 count;
    /* 0x04 */ u32 capacity;
    /* 0x08 */ u8 pad8[0x38];
    /* 0x40 */ GphTexSlot slots[1]; /* capacity entries */
} GphTexTable;

/* -------------------------------------------------------- 2D anim sets */

/* Cell (GphAnimGetCellUv 452B18, copied as one qword into particle UVs). */
typedef struct GphAnimCell {
    /* 0x00 */ u8 data[0x10];
} GphAnimCell; /* size = 0x10 */

/* Cell rectangle as GphAnimGetCellRect 452B58 reads it (8-byte stride over
 * the same cell pointer, used by 452BB8). */
typedef struct GphAnimRect {
    /* 0x0 */ u16 x;
    /* 0x2 */ u16 y;
    /* 0x4 */ u16 w;
    /* 0x6 */ u16 h;
} GphAnimRect; /* size = 0x8 */

/* Key (GphAnimGetKey 4531D8). */
typedef struct GphAnimKey {
    /* 0x00 */ u16 cellStart;      /* GphAnimStep 4532C0 */
    /* 0x02 */ u16 cellEnd;
    /* 0x04 */ u16 loops;
    /* 0x06 */ u16 wait;           /* duration (452BB8 "dur") */
    /* 0x08 */ s16 x;              /* offset (GphAnimBuildSprite 452BB8) */
    /* 0x0A */ s16 y;
    /* 0x0C */ s16 rot;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ u32 color;
    /* 0x14 */ f32 scaleX;
    /* 0x18 */ f32 scaleY;
    /* 0x1C */ u8 pad1C[4];
} GphAnimKey; /* size = 0x20 */

/* Anim set header (GphAnimSetRelocate 452AB8 turns +4/+8/+0xC into pointers;
 * 452AB8 views them as s32, 452B18/452B58 view +0..+3 as u16 unk0, u16 count). */
typedef struct GphAnimSet {
    /* 0x00 */ u8 flags;           /* 2 one-shot, 4 (452BB8), 8 random start */
    /* 0x01 */ u8 keyCount;
    /* 0x02 */ u16 cellCount;
    /* 0x04 */ union {
                   GphAnimCell *cells;
                   GphAnimRect *rects; /* GphAnimGetCellRect's view of the same pointer */
               } cell;
    /* 0x08 */ GphAnimKey *keys;
    /* 0x0C */ GphTex *tex;        /* texture */
} GphAnimSet; /* size = 0x10 */

/* Per-sprite anim state (GphAnimStateInit 453218, GphAnimStep 4532C0). */
typedef struct GphAnimState {
    /* 0x0 */ u8 done;
    /* 0x1 */ u8 cell;
    /* 0x2 */ u8 key;
    /* 0x3 */ u8 rep;
    /* 0x4 */ f32 time;
} GphAnimState; /* size = 0x8 */

/* ------------------------------------------------------- 2D batch quads */

/* Corner of a batch primitive. GphBatchDrawQuads 449148 sends +4/+6 to the
 * GS UV register and +8/+A to XYZ2. */
typedef struct GphBatchVtx {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
    /* 0x3 */ u8 a;
    /* 0x4 */ u16 u;            /* texels */
    /* 0x6 */ u16 v;
    /* 0x8 */ s16 x;            /* screen pixels, offset by gphBatchOffsetX/Y */
    /* 0xA */ s16 y;
} GphBatchVtx; /* size = 0xC */

/* Primitive of a GphBatchDrawQuads / DrawQuadsColor / DrawSprites list (a
 * sprite uses corners 0 and 1). A list ends at the first type 0; types 8 and
 * up are skipped. */
typedef struct GphBatchQuad {
    /* 0x00 */ u32 type;        /* blend state gphBatchBlendStates[type - 1]; 0 = end */
    /* 0x04 */ u8 pad4[8];
    /* 0x0C */ s32 z;
    /* 0x10 */ GphBatchVtx v[4];
} GphBatchQuad; /* size = 0x40 */

/* Billboard quad for GphQuad3DSetCorners / GphBatchDrawQuads3D 449B48, an
 * array ended by type 0. pos is not 16-byte aligned (449B48 passes it to
 * VuProjectVerts as is). */
#ifndef GPH_QUAD3D_DEFINED
#define GPH_QUAD3D_DEFINED
typedef struct GphQuad3DCol {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
    /* 0x3 */ u8 a;
} GphQuad3DCol; /* size 0x4 */

typedef struct GphQuad3D {
    /* 0x00 */ u32 type;           /* blend state gphBatchBlendStates[type - 1] (0xD textured); 0 = end */
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ VuVec4 pos[4];      /* corners (GphQuad3DSetCorners) */
    /* 0x50 */ GphQuad3DCol col[4];
    /* 0x60 */ u16 uv[4][2];       /* u, v per corner */
} GphQuad3D; /* size 0x70 */
#endif

/* ----------------------------------------------------------- screen fades */

/* Fade object (GphFadeClear/SetColor/Start/Update/Draw 0x00451EE8-
 * 0x00452258; gphScreenFade, FIELD's fldFades[2], SysDemo.fade/fade2). */
#ifndef GPH_FADE_DEFINED
#define GPH_FADE_DEFINED
typedef struct GphFade {
    /* 0x000 */ u8 active;
    /* 0x001 */ u8 mode;           /* 0 = draw (GphFadeDraw), else stop */
    /* 0x002 */ u8 unk2;           /* blend: primitive type ((unk2 + 1) & 3) | 4 */
    /* 0x003 */ u8 unk3;           /* target alpha as an int (GphFadeStart) */
    /* 0x004 */ f32 color[4];      /* RGBA */
    /* 0x014 */ f32 delta[4];      /* per-frame step */
    /* 0x024 */ f32 time;          /* duration in frames */
    /* 0x028 */ f32 count;         /* frames done (stored as s32 0 by GphFadeStart) */
    /* 0x02C */ u8 pad2C[4];
    /* 0x030 */ GphBatchQuad prim[2]; /* full-screen quad + end marker */
    /* 0x0B0 */ u8 padB0[0x80];
} GphFade; /* size 0x130 */
#endif

/* ------------------------------------------------------- screen shatter */

/* Shatter transition state (gphShatter, GphShatter* 0x0040C670-0x0040D860). */
typedef struct GphShatter {
    /* 0x000 */ s32 active;
    /* 0x004 */ u16 mode;          /* 0, 1, 3, 9, 10 */
    /* 0x006 */ u16 timer;
    /* 0x008 */ s8 unk8;
    /* 0x009 */ u8 pad9[3];
    /* 0x00C */ f32 unkC;
    /* 0x010 */ f32 unk10;
    /* 0x014 */ GphModel *model;   /* gphShatterScene; pieces = its parts */
    /* 0x018 */ u8 pad18[8];
    /* 0x020 */ f32 color[4];      /* [3] fade alpha; the pieces' GA objects point at it (+0x38) */
    /* 0x030 */ u8 pad30[0x50];
    /* 0x080 */ GphBatchQuad prim[2]; /* gphShatterPrim, full-screen captured image + end marker */
    /* 0x100 */ u8 pad100[0x180];
    /* 0x280 */ u8 modelPool[0x40]; /* ChrModelPoolSet */
} GphShatter; /* size = 0x2C0 (gphShatterPieces follows) */

/* One piece's motion (gphShatterPieces, 0x50 each). */
typedef struct GphShatterPiece {
    /* 0x00 */ FVECTOR pos;
    /* 0x10 */ FVECTOR vel;        /* .x/.z s32 in 40CC70/40CF40 (they only store 0) */
    /* 0x20 */ IVECTOR rot;
    /* 0x30 */ IVECTOR rotVel;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ u8 pad44[0xC];
} GphShatterPiece; /* size = 0x50 */

/* --------------------------------------------------------- screen effects */

/* GphScreenFx.flags */
#define GPH_SFX_BLUR    0x2     /* type 1: blur, lerped to blurTo */
#define GPH_SFX_BOKEH   0x4     /* type 2: GphFbBokeh */
#define GPH_SFX_WARP    0x8     /* type 3: GphFbMeshWarp ("Blow") */
#define GPH_SFX_FADE    0x10    /* type 4: captured frame faded out */
#define GPH_SFX_RIPPLE  0x200   /* type 5: GphFbRippleDraw */
#define GPH_SFX_STILL   0x400   /* type 6: captured frame shown */
#define GPH_SFX_BLURFIX 0x800   /* type 7: blur of a fixed amount */
#define GPH_SFX_HOLD    0x8000  /* GphScreenFxSetHold: keep blur/fade when done */

/*
 * Screen-effect controller (GphScreenFxSet 453EB8, GphScreenFxUpdate 4543D0,
 * GphScreenFxDraw 454290; FIELD's fldScreenFx[2], BATTLE's btlScreenFx).
 * Each lerped effect has from/to/t/step/current; the current value is what
 * GphScreenFxDraw uses.
 */
#ifndef GPH_SCREEN_FX_DEFINED
#define GPH_SCREEN_FX_DEFINED
typedef struct GphScreenFx {
    /* 0x00 */ s32 flags;          /* GPH_SFX_*; clear masks stay unsigned literals */
    /* 0x04 */ f32 bokehFrom;
    /* 0x08 */ f32 bokehTo;
    /* 0x0C */ f32 bokeh;
    /* 0x10 */ f32 bokehT;
    /* 0x14 */ f32 bokehStep;      /* 1 / frames */
    /* 0x18 */ f32 warpFrom;
    /* 0x1C */ f32 warpTo;
    /* 0x20 */ f32 warpT;
    /* 0x24 */ f32 warpStep;
    /* 0x28 */ f32 warp;
    /* 0x2C */ f32 blurTo;
    /* 0x30 */ f32 blurFrom;
    /* 0x34 */ f32 blurT;
    /* 0x38 */ f32 blurStep;
    /* 0x3C */ f32 blur;           /* GPH_SFX_BLURFIX sets it directly */
    /* 0x40 */ f32 fadeT;
    /* 0x44 */ f32 fadeStep;
    /* 0x48 */ f32 fadeAlpha;      /* 128 -> 0 */
    /* 0x4C */ u8 pad4C[4];
    /* 0x50 */ f32 rippleAngle;    /* += 0.377 per frame up to 20 pi */
    /* 0x54 */ u8 pad54[4];
} GphScreenFx; /* size 0x58 */
#endif

/* --------------------------------------------------- frame-buffer effects */

/* FB effect parameters (gphFbDivParam), FRAMEBUFFER MENU pages 1-3. */
typedef struct GphFbDivParam {
    /* 0x00 */ f32 centerView;     /* CENTER VIEW (1.0 = on); 423D38 views f32[] by line */
    /* 0x04 */ f32 nineDivide;     /* 9 DIVIDE */
    /* 0x08 */ f32 nineDivScrollV; /* 9 DIVIDE UD SCRL */
    /* 0x0C */ f32 nineDivScrollH; /* 9 DIVIDE LR SCRL */
    /* 0x10 */ f32 mirror;         /* MIRROR */
    /* 0x14 */ u8 pad14[0x2C];
    /* 0x40 */ f32 bokeh;          /* Boken */
    /* 0x44 */ f32 blow;           /* Blow */
    /* 0x48 */ f32 theta;          /* THEATA */
    /* 0x4C */ f32 ripplePhase;
    /* 0x50 */ s16 x;              /* SYSTEM 2 rect X/Y/W/H/U/V */
    /* 0x52 */ s16 y;
    /* 0x54 */ s16 w;
    /* 0x56 */ s16 h;
    /* 0x58 */ s16 u;
    /* 0x5A */ s16 v;
    /* 0x5C */ u8 rippleTrigger;   /* also the symbol sysNewGameFlag */
    /* 0x5D */ u8 system2Mode;     /* SYSTEM 2 MODE; gphShatterHold */
    /* 0x5E */ u8 blur;            /* Blur */
    /* 0x5F */ u8 pad5F[2];
    /* 0x61 */ u8 multiMode;       /* MULTI MODE */
    /* 0x62 */ u8 multiW;
    /* 0x63 */ u8 multiH;
    /* 0x64 */ u8 aMode;           /* A MODE */
    /* 0x65 */ u8 alpha;           /* ALPHA */
    /* 0x66 */ u8 r[4];            /* P0..P3 R */
    /* 0x6A */ u8 g[4];
    /* 0x6E */ u8 b[4];
    /* 0x72 */ u8 a[4];
    /* 0x76 */ u8 pad76[0x32];
} GphFbDivParam; /* size = 0xA8 (up to sysState) */

typedef struct GphFbSt {
    /* 0x0 */ f32 s;
    /* 0x4 */ f32 t;
} __attribute__((aligned(8))) GphFbSt; /* size = 0x8 */

/* FB effect packet buffer (gphFbPacket = scratchpad 0x70003C00), as the
 * effects that draw one sprite use it. */
/* GphFbPacket's texture load packet (GsTexLoadPacket); the callers then
 * patch its TEX0 register in place. */
typedef union GphFbTexLoad {
    GsTexPacket pkt;
    struct {
        /* 0x00 */ u8 pad0[0x30];
        /* 0x30 */ u64 tex0;
    } reg;
} GphFbTexLoad; /* size 0x140 */

typedef struct GphFbPacket {
    /* 0x000 */ u8 pad0[0x80];       /* draw environment (GsDrawEnvPacketSet) */
    /* 0x080 */ u8 renderState[0x30]; /* GsRenderStatePacketSet at +0x80 (444270) */
    /* 0x0B0 */ u64 unkB0;           /* the render state's PRMODE (442BB8 sets ABE) */
    /* 0x0B8 */ u8 padB8[0x88];
    /* 0x140 */ u8 renderState2[0x90]; /* a second GsRenderStatePacketSet packet (445200) */
    /* 0x1D0 */ u8 pad1D0[0x30];
    /* 0x200 */ GphFbTexLoad texLoad;
    /* 0x340 */ u64 dmaTag;          /* 0x10000005 (444270) */
    /* 0x348 */ u64 dmaTagHi;
    /* 0x350 */ u32 vif[4];          /* ... DIRECT */
    /* 0x360 */ u64 gifTag;
    /* 0x368 */ u64 gifRegs;
    /* 0x370 */ u64 prim;
    /* 0x378 */ u64 rgbaq;
    /* 0x380 */ GphFbSt st0;         /* u64 in 4437C0/444270 (GsPackST returning u64) */
    /* 0x388 */ u64 xyz0;
    /* 0x390 */ GphFbSt st1;
    /* 0x398 */ u64 xyz1;
} GphFbPacket; /* size = 0x3A0 */

/* The same buffer as GphFbMeshWarp 4437C0 fills it with triangle strips
 * after the sprite pass. A separate type, not a union member of
 * GphFbPacket: 4437C0 only matches with the strip stores going through
 * their own struct type. */
typedef struct GphFbStripPacket {
    /* 0x000 */ u8 pad0[0x340];
    /* 0x340 */ u64 dmaTag;
    /* 0x348 */ u64 dmaTagHi;
    /* 0x350 */ u32 vif[4];
    /* 0x360 */ u64 gifTag;
    /* 0x368 */ u64 gifRegs;
    /* 0x370 */ u64 prim;
    /* 0x378 */ struct {
                   u64 rgbaq;
                   u64 st;
                   u64 xyz;
               } v[4];
    /* 0x3D8 */ u64 end;
} GphFbStripPacket; /* size = 0x3E0 */

/* ----------------------------------------------------------------- globals */

extern GphLightList *gphLightList;       /* 0x004B6300; u8 * in 4321C0, s32 in 4320F0 */
extern GphView gphViews[8];              /* 0x004B52C0; u8[] in 42F608 */
extern GphTexTable *gphTexTable;         /* 0x004B7088 */
extern GphTexTable *gphTexTableBuf;      /* 0x004A9448, the 256-slot table (s32 in the overlays) */
extern GphAnimSet *gphCurAnimSet;        /* 0x004A94AC */
extern GphShatter gphShatter;            /* 0x004A9AC0 */
extern GphShatterPiece *gphShatterPieces; /* 0x004A9D80 */
extern GphTex gphFbTexInfo;              /* 0x0049AA80 */
extern GphTex gphFbUploadTexInfo;        /* 0x0049AB00 */
extern GphFbPacket *gphFbPacket;         /* 0x004B70B0 */
extern GphFbDivParam gphFbDivParam;      /* 0x004F0C58 */
extern s32 gphFbEffParam[8];             /* 0x004F0AB8 - VWAVE TYPE, DIV, MAX, ADD, CENT, CNT, SPEED, COLOR */

#endif
