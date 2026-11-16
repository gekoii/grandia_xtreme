/* main executable: the memory card module's tables (.data) and their
 * strings (.rodata). Data-only C (plan step 6b, docs/shifting.md); the
 * functions that use them (42C330.c and its neighbours) are still separate
 * files. The .rodata range starts with the header's "save" string, so it is
 * the start of this module's .rodata; the string literals of the name
 * tables follow in the order gcc 2.9 emits them (each table's new strings
 * after the table, last one first; a string used twice is emitted once).
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x0049A848-0x0049A938 */
/* rodata: 0x004A5948-0x004A5A30 */

typedef unsigned char u8;
typedef unsigned int u32;

/* .bss */
extern u8 mcIconSysBuf[];      /* icon.sys image, built at run time */
extern u8 mcBlankSaveData[];
extern u8 mcDirListBuf[];      /* directory listing */

/* A header's unused static string: the first thing in each file's .rodata.
 * The memory card code compares save file names with it, so here it is a
 * global with that name. */
const char mcSaveNamePrefix[] = "save";

/* The files of a save, written in this order */
typedef struct {
    char name[0x20];
    u32 size;
    void *data; /* filled in at run time (42C330) except for the save data */
} McFileEntry;

/* fields: mcIconSysSize [0].size, mcIconSysData [0].data,
 * mcIconFileEntry [1], mcIconFileSize [1].size,
 * mcSaveFileEntry [2], mcSaveDataSize [2].size, mcSaveDataPtr [2].data */
McFileEntry mcIconSysEntry[3] = {
    { "icon.sys", 0x3C4, mcIconSysBuf },
    { "icon0.ico", 0xB638 },
    { "BASLUS-20417GrandiaX", 0x400, mcBlankSaveData }, /* renamed to saveNN.dat */
};

u32 mcAccessMode = 0;           /* Save/Load/Boot */
void *mcDirListPtr = mcDirListBuf;

/* libmc command names, indexed by the card-info command */
char *mcCmdNames[22] = {
    "NONE", "CardInfo", "Open", "Close", "Seek", "Read", "Write", 0,
    0, 0, "Flush", "Mkdir", "ChDir", "GetDir", "FileInfo", "Delete",
    "Format", "Unformat", "EntSpace", "Rename", "ChgPrior", 0,
};

char *mcCardTypeNames[4] = { "NONE", "PS", "PS2", "PDA" };

char *mcFormatNames[2] = { "UnFormat", "Format" };
