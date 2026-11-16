/* main executable .data 0x0049D6A8-0x0049DD70: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/09A938.data.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* data: 0x0049D6A8-0x0049DD70 */

typedef int s32;
typedef float f32;

/* "MOV/TRP6.BIN".."MOV/TRP0.BIN", the movie module's .rodata (the table's
 * literals, last element first). */
extern char movTrpFileName6[];
extern char movTrpFileName5[];
extern char movTrpFileName4[];
extern char movTrpFileName3[];
extern char movTrpFileName2[];
extern char movTrpFileName1[];
extern char movTrpFileName0[];

/* A MovPlayList playlist (include/main/n11.h MovPlaylist). */
typedef struct MovPlaylist {
    s32 count;         /* files */
    char name[6][0x16];
    s32 repeat[6];     /* times each file is played */
    void *base[6];     /* where it is read to, set by MovPlayList */
    s32 size[6];       /* file sizes, set by MovPlayList */
} MovPlaylist; /* size = 0xD0 */

/* Overlay timings of playlist types 0-5 (MovListVblankHandler, 0046B730). */
typedef struct MovOverlayParam {
    s32 endFrame;
    s32 scrollHeight;
    s32 stripLength;
    s32 scrollStart;
    f32 scrollSpeed;
    s32 stripStart;
    f32 stripSpeed;
} MovOverlayParam; /* size = 0x1C */

MovPlaylist movPlaylistTable[7] = {
    {
        5,
        { "MOV/SOLONT.PSS", "MOV/STAGE01A.PSS", "MOV/STAGE01B.PSS", "MOV/STAGE01C.PSS",
          "MOV/STAGE01D.PSS" },
        { 1, 4, 4, 4, 5 },
    },
    {
        5,
        { "MOV/SOLONT.PSS", "MOV/STAGE02A.PSS", "MOV/STAGE02B.PSS", "MOV/STAGE02C.PSS",
          "MOV/STAGE02D.PSS" },
        { 1, 8, 8, 8, 8 },
    },
    {
        3,
        { "MOV/SOLONT.PSS", "MOV/STAGE03A.PSS", "MOV/STAGE03B.PSS" },
        { 1, 10, 11 },
    },
    {
        4,
        { "MOV/SOLONT.PSS", "MOV/STAGE04A.PSS", "MOV/STAGE04B.PSS", "MOV/STAGE04C.PSS" },
        { 1, 9, 9, 8 },
    },
    {
        4,
        { "MOV/EARTH.PSS", "MOV/STAGE05A.PSS", "MOV/STAGE05B.PSS", "MOV/STAGE05C.PSS" },
        { 1, 4, 5, 5 },
    },
    {
        5,
        { "MOV/EARTH.PSS", "MOV/STAGE06A.PSS", "MOV/STAGE06B.PSS", "MOV/STAGE06C.PSS",
          "MOV/STAGE06D.PSS" },
        { 1, 6, 7, 6, 7 },
    },
    {
        1,
        { "MOV/IPART.MV2" },
        { 1 },
    },
}; /* 0049D6A8; fields: D_0049D6AC +0x4 ([0].name), D_0049D748 +0xA0 ([0].base), D_0049D760 +0xB8 ([0].size) */
char *movTrpFileTable[7] = {
    movTrpFileName0, movTrpFileName1, movTrpFileName2, movTrpFileName3,
    movTrpFileName4, movTrpFileName5, movTrpFileName6,
}; /* 0049DC58 */
s32 movAudioHeld = 0; /* 0049DC74 */
s32 movReadState = 0; /* 0049DC78 */
s32 movReadSectors = 0; /* 0049DC7C */
s32 movListReadState = 0; /* 0049DC80 */
s32 movListReadSectors = 0; /* 0049DC84 */
MovOverlayParam movOverlayParamTable[6] = {
    { 1957, 242, 260, 200, 0.184159994f, 230, 3.8118f },
    { 3867, 494, 546, 200, 0.156749994f, 230, 3.81900001f },
    { 2564, 310, 390, 200, 0.163770005f, 230, 4.3915801f },
    { 3122, 410, 494, 200, 0.166761994f, 230, 4.33665991f },
    { 1684, 188, 312, 200, 0.181508005f, 200, 5.34817982f },
    { 3161, 408, 520, 200, 0.164829999f, 230, 4.43981981f },
}; /* 0049DC88 */
s32 movFrameStarve = 0; /* 0049DD30 */
s32 movOvlFieldCount = 0; /* 0049DD34 */
s32 movOvlScrollStep = 0; /* 0049DD38 */
s32 movOvlStripStep = 0; /* 0049DD3C */
s32 movOvlPictureStep = 0; /* 0049DD40 */
s32 movOvlLabelFadeStep = 0; /* 0049DD44 */
s32 movOvlDimFadeStep = 0; /* 0049DD48 */
s32 movSkipFadeStep = 0; /* 0049DD4C */
s32 movDispActive = 0; /* 0049DD50 */
s32 movVblankCount = 0; /* 0049DD54 */
s32 movFrameSent = 0; /* 0049DD58 */
s32 movOddField = 0; /* 0049DD5C */
s32 movPathBusy = 0; /* 0049DD60 */
s32 movSkipFadeMode = 1; /* 0049DD64; the zeros up to 0049DD70 are libgraph's 16-alignment */
