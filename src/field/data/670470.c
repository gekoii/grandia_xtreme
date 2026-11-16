/* FIELD .data 0x00670470-0x00670748: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0B6788.data.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* data: 0x00670470-0x00670748 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef f32 FVECTOR[4] __attribute__((aligned(16))); /* include/vu.h */

typedef struct UV {
    u8 u, v;
} UV;

/* {flag id, value} (FldFlagListOpen). */
typedef struct FldFlagListEntry {
    u16 id;
    u16 value;
} FldFlagListEntry;

extern u8 mcSpaceCheckDirBuf[]; /* McDirList (include/main/n03.h) */

/* Selected light on the point light debug page (0x00618CB0). */
u32 fldPointLightDbgSel = 0; /* 00670470 */
/* btlHandoff+0x5E start type by initiative (FldBattleStart 0x00619C80 reads
 * [0] and [1] and adds a bonus); the rest unread: pairs +8 apart. */
u8 fldBtlStartTypeNormal[8] = { 0, 2, 8, 10, 16, 18, 24, 26 }; /* 00670478; fields: fldBtlStartTypeAmbush +0x1 */
/* Unit kinds (0x1001.. party, then enemies) with a GBX entry, index*4 into
 * sysGbxPac (FldBattleFindEnemyGbx 0x006197E8). */
u16 fldBtlGbxKinds[47] = {
    0x1001, 0x1002, 0x1003, 0x1004, 0x1005, 0x1006, 0x1007, 0x1008,
    0x4, 0x9, 0xD, 0x18, 0x19, 0x23, 0x25, 0x2B,
    0x41, 0x51, 0x61, 0x64, 0x65, 0x71, 0x81, 0x91,
    0x92, 0xA1, 0xC1, 0xD1, 0x104, 0x109, 0x10D, 0x118,
    0x119, 0x123, 0x125, 0x12B, 0x151, 0x161, 0x164, 0x165,
    0x171, 0x181, 0x1A1, 0x204, 0x20D, 0x218, 0x219
}; /* 00670480 */
/* FldCollPartGatherSphere's triangle counts: tested, passing
 * VuSphereTriBoundHit, VuTriPlaneNear and VuPointInTriangle (added). */
s32 fldCollTriTestCount = 0; /* 006704E0 */
s32 fldCollTriBoundCount = 0; /* 006704E4 */
s32 fldCollTriPlaneCount = 0; /* 006704E8 */
s32 fldCollTriHitCount = 0; /* 006704EC */
s32 fldCollStat10 = 0; /* 006704F0: cleared by FldCollGather, otherwise unused */
/* The 8 corners of a +-0.5 cube (FldCollShapeBoxTest). */
FVECTOR fldCollUnitBoxCorners[8] = {
    { 0.5f, 0.5f, 0.5f, 1.0f },
    { -0.5f, 0.5f, 0.5f, 1.0f },
    { -0.5f, 0.5f, -0.5f, 1.0f },
    { 0.5f, 0.5f, -0.5f, 1.0f },
    { 0.5f, -0.5f, 0.5f, 1.0f },
    { -0.5f, -0.5f, 0.5f, 1.0f },
    { -0.5f, -0.5f, -0.5f, 1.0f },
    { 0.5f, -0.5f, -0.5f, 1.0f },
}; /* 00670500 */
/* u, v of the top menu's command icons (the first 6 used by 0x0061FC28). */
UV fldMenuCommandIconUVs[13] = {
    { 0x44, 0x00 },
    { 0x00, 0x22 },
    { 0x00, 0x66 },
    { 0x22, 0x44 },
    { 0x44, 0x22 },
    { 0x44, 0x44 },
    { 0x66, 0x00 },
    { 0x00, 0x00 },
    { 0x22, 0x00 },
    { 0x22, 0x22 },
    { 0x66, 0x22 },
    { 0x00, 0x44 },
    { 0x66, 0x44 },
}; /* 00670580 */
/* Window id per screen 1 2x2 grid cell (0x00626CC8). */
u8 fldMenuScreen1WinIds[4] = { 4, 5, 2, 3 }; /* 006705A0 */
/* Guide mode per item category (0x00627260 reads [0]). */
u8 fldMenuItemGuideModes[6] = { 2, 8, 8, 8, 9, 9 }; /* 006705A8 */
/* Dialog window 0xB open (FldMenuMcDlgOpen/Close). */
s32 fldMenuMcDlgIsOpen = 0; /* 006705B0 */
/* SE id base per Carro kind (MiniConcertCarroSePlay 0x0064C238). */
u16 miniConcertCarroSeBase[5] = { 0x81B, 0x820, 0x825, 0x82A, 0x82F }; /* 006705B8 */
/* Peak draw time shown by MiniConcertDraw in debug mode. */
f32 miniConcertDbgTimePeak = 0.0f; /* 006705C4 */
/* 11 spline keys (x, y, z, time) for map object 2's animated part
 * (WmapPathEval 0x00655F20, from WmapUpdate). */
FVECTOR wmapPathKeys[11] = {
    { 0.0f, 220.0f, 0.0f, -30.0f },
    { 10.0f, 200.0f, 0.0f, 0.0f },
    { 0.0f, 180.0f, 10.0f, 20.0f },
    { -10.0f, 170.0f, 0.0f, 40.0f },
    { 0.0f, 160.0f, 0.0f, 60.0f },
    { 10.0f, 170.0f, 0.0f, 80.0f },
    { 0.0f, 180.0f, -10.0f, 100.0f },
    { -10.0f, 200.0f, 0.0f, 120.0f },
    { 0.0f, 220.0f, 0.0f, 140.0f },
    { 10.0f, 200.0f, 0.0f, 160.0f },
    { 0.0f, 180.0f, 0.0f, 180.0f },
}; /* 006705D0 */
f32 wmapUnusedLevel = 128.0f; /* 00670680: written by WmapStart/WmapControl, never read */
f32 wmapPathTime = 0.0f; /* 00670684: time along wmapPathKeys, 0..160 looping */
u16 wmapStickRepeat = 0; /* 00670688: analog stick repeat counter (WmapReadDir) */
s32 wmapEnterTimer = 0; /* 0067068C: frames of the enter transition (map jump at 120) */
s32 wmapDbgCamEdit = 0; /* 00670690: debug camera edit on/off */
s32 wmapDbgCamMode = 0; /* 00670694: debug camera edit target - 0 target, 1 position */
/* Entries of the flag list debug page (FldFlagListOpen). */
FldFlagListEntry fldFlagListEntries[0x26] = {
    { 0x3FD, 0x00 }, { 0x3F5, 0x01 }, { 0x3FE, 0x02 }, { 0x3F6, 0x03 },
    { 0x3F7, 0x04 }, { 0x3D0, 0x05 }, { 0x3D1, 0x06 }, { 0x3D2, 0x07 },
    { 0x3D3, 0x08 }, { 0x3D4, 0x09 }, { 0x3D5, 0x0A }, { 0x3F8, 0x0B },
    { 0x3F9, 0x0C }, { 0x3FA, 0x0D }, { 0x3FB, 0x25 }, { 0x3D6, 0x0F },
    { 0x3D7, 0x10 }, { 0x3FF, 0x0E }, { 0x3D9, 0x11 }, { 0x3DA, 0x12 },
    { 0x3DB, 0x13 }, { 0x3DC, 0x14 }, { 0x3DD, 0x15 }, { 0x3DE, 0x16 },
    { 0x3DF, 0x17 }, { 0x3E0, 0x18 }, { 0x3E1, 0x19 }, { 0x3E2, 0x1A },
    { 0x3E3, 0x1B }, { 0x3E4, 0x1C }, { 0x3E5, 0x1D }, { 0x3E6, 0x1E },
    { 0x3E7, 0x1F }, { 0x3E8, 0x20 }, { 0x3E9, 0x21 }, { 0x3EA, 0x22 },
    { 0x3EB, 0x23 }, { 0x3EC, 0x24 },
}; /* 00670698 */
/* Memory card space check (McSpaceCheck*): 0 port 0, 1 port 1, 2/3
 * messages, 4 done; previous states; per-port check state 0x40x. */
s32 mcSpaceCheckState = 0; /* 00670730 */
s32 mcSpaceCheckPrevState = -1; /* 00670734 */
s32 mcSpaceCheckPortPrevState = -1; /* 00670738 */
s32 mcSpaceCheckPortState = 0; /* 0067073C */
void *mcSpaceCheckDirList = mcSpaceCheckDirBuf; /* 00670740: directory listing for McSpaceCheckReadDir */
u32 evtScnrStreamSyncStep = 0; /* 00670744: state 0-4 of EvtScnrStreamSyncUpdate */
