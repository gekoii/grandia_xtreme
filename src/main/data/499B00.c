/* main executable: the start of .data - the program (overlay) table and the
 * movie file names. Data-only C (plan step 6b, docs/shifting.md); the
 * functions that use them are 400988.c, 400A10.c, 400BD8.c and 401160.
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x00499B00-0x00499D80 */

typedef unsigned char u8;

/* Load addresses (config/memory_map.ld): each program's entry point is its
 * first byte. Only FIELD, BATTLE and VIEWER are on the disc; the other
 * programs would share VIEWER's slot. */
extern u8 FIELD_VRAM[];
extern u8 BATTLE_VRAM[];
extern u8 SUBPROG_VRAM[];

typedef struct {
    char file[0x10];        /* file name on the disc */
    u8 *addr;               /* load address = entry, called as callback 0 */
    void (*callbacks[8])(); /* stored by the program's entry (SysProgRegister) */
} SysProgram;

/* SysProgLoad(n) loads sysProgramTable[n].file to .addr and calls it */
SysProgram sysProgramTable[7] = {
    { "VIEWER.BIN", SUBPROG_VRAM },
    { "TITLE.BIN", SUBPROG_VRAM },
    { "FIELD.BIN", FIELD_VRAM },
    { "BATTLE.BIN", BATTLE_VRAM },
    { "ENDING.BIN", SUBPROG_VRAM },
    { "MCCHECK.BIN", SUBPROG_VRAM },
    { "AUTOMAP.BIN", SUBPROG_VRAM },
};

/* SysMoviePlay(n) plays sysMovieFileTable[n] */
char sysMovieFileTable[16][17] = {
    "MOVIE/GRM00.MOV",
    "MOVIE/GRM01.MOV",
    "MOVIE/GRM02.MOV",
    "MOVIE/GRM03.MOV",
    "MOVIE/GRM04.MOV",
    "MOVIE/GRM05.MOV",
    "MOVIE/GRM06.MOV",
    "MOVIE/GRM07.MOV",
    "MOVIE/GRM08.MOV",
    "MOVIE/GRM09.MOV",
    "MOVIE/GRM10.MOV",
    "MOVIE/GRM11.MOV",
    "MOVIE/GRM12.MOV",
    "MOVIE/GRM13.MOV",
    "MOVIE/GRM14.MOV",
    "MOVIE/GRM15.MOV",
};
