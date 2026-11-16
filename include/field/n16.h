#ifndef FIELD_N16_H
#define FIELD_N16_H

/*
 * FIELD 0x005D3758-0x005E0950: scenario map flags, treasure pickups, the
 * field debug menu and the map effect spawners. Globals and helper types
 * shared by several files of this range that no wider header covers.
 */

#include "common.h"
#include "vu.h"

/* ---------------------------------------------------------------- map flags */

/* evtCurMapNo (= sysSaveData.mapNo) is in save.h. */

/* One map's local flags (EvtMapFlagGet/Set) and treasure-taken flags
 * (EvtMapTreasureFlagGet/Set); slot from EvtMapFlagSlot. */
typedef struct EvtMapFlags {
    /* 0x0 */ u8 flags[8];         /* 64 local flags */
    /* 0x8 */ u8 treasureFlags[4]; /* 32 treasure-taken flags */
} EvtMapFlags; /* size 0xC */

extern EvtMapFlags *evtMapFlagTable; /* slots 0..8 (EvtMapFlagSlot); EvtAreaStateReset clears 0x90 bytes */

/* ---------------------------------------------------------------- helpers */

/* FldEncGroup (fldEncGroups entries) is in field/state.h. */

/* The current map's set file (MapSetFileGet), field names from the debug
 * SETFILE page (DbgFldSetFilePage); FldTreasurePlace caps the random
 * treasures with itNum1/itNum2. */
#ifndef MAP_SET_FILE_DEFINED
#define MAP_SET_FILE_DEFINED
typedef struct MapSetFileMonster {
    /* 0x0 */ s16 id;              /* "id $%03X" */
    /* 0x2 */ s16 lvMax;
    /* 0x4 */ s16 lvMin;
    /* 0x6 */ s8 rate;
    /* 0x7 */ s8 num;
    /* 0x8 */ s8 leader;
    /* 0x9 */ s8 cmm;
    /* 0xA */ u8 padA[2];
} MapSetFileMonster; /* size 0xC */

typedef struct MapSetFile {
    /* 0x00 */ u8 no;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ u16 mapNo;
    /* 0x04 */ s32 goldMax;
    /* 0x08 */ s32 goldMin;
    /* 0x0C */ s32 trapDmg3;
    /* 0x10 */ s32 trapDmg2;
    /* 0x14 */ s32 trapDmg1;
    /* 0x18 */ u16 btlFld;
    /* 0x1A */ u8 itNum1;          /* item treasures (set file types 2/3) */
    /* 0x1B */ u8 itRank1;
    /* 0x1C */ u8 itNum2;          /* types 4/5 */
    /* 0x1D */ u8 itRank2;
    /* 0x1E */ s8 mcNum;
    /* 0x1F */ s8 goldNum;
    /* 0x20 */ MapSetFileMonster monsters[16];
} MapSetFile; /* size 0xE0 */
#endif

/* ---------------------------------------------------------------- debug menu */

/* Strings of the field debug pages (DbgPrintf formats and values). */
extern char dbgFldOffOnStr[][4];     /* "OFF", "ON", indexed by a flag */
extern char dbgFldRunPauseStr[][6];  /* "RUN", "PAUSE" (CAMERA PAUSE) */
extern char dbgFldStopRunStr[][6];   /* "STOP", "RUN" (CAMERA STATUS) */
extern char *dbgFldOccStatusNames[]; /* 8 OCC/OCCS status names */
extern char dbgFldPageColor5Str[];   /* "\c5 " page-number colour */
extern char dbgFldPageColor0Str[];   /* "\c0 " colour reset */
extern char dbgMainFlagStrCameraPause[];
extern char dbgMainFlagStrCameraStatus[];
extern char dbgChrStrGroupMax[];
extern char dbgChrStrGroupPoint[];
extern char dbgChrStrGroupInfHeader[];
extern char dbgChrStrGroupNo[];
extern char dbgChrStrGroupMaxRow[];
extern char dbgChrStrGroupLevel[];
extern char dbgOccStrCount[];
extern char dbgOccStrGrp[];
extern char dbgOccStrIndex[];
extern char dbgOccStrStatus[];
extern char dbgOccStrAttribute[];
extern char dbgOccStrAutoWakeOn[];
extern char dbgOccStrAutoWakeOff[];
extern char dbgOccStrLoopOn[];
extern char dbgOccStrLoopOff[];

/* Debug state (DbgFldInit). */
extern u16 dbgFldDispLines;          /* ">LINE" DISP LINES bit mask (DbgFldLinesDraw) */
extern s32 dbgFldPolygonCount;
extern VuVec4 dbgFldMoveCursorPos;  /* CHARA MOVE cursor (DbgFldCursorDraw) */
extern f32 dbgFldMoveCursorDir;      /* degrees */
extern s32 dbgMusicSeek;             /* SYSTEM page "MUSIC SEEK" (0/1), passed to SndBgmSetSeek */

#endif /* FIELD_N16_H */
