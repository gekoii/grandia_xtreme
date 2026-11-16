/*
 * The player's disc image (port/src/disc.c) and the game's disc drivers on
 * top of it (port/src/cdvd.c). See port/docs/disc.md.
 *
 * Only for the port's own sources (port/src/, port/tests/): the game's C
 * never sees these names. Nothing read from the image is ever written
 * anywhere but the game's own memory.
 */
#ifndef GRX_DISC_H
#define GRX_DISC_H

#include <stdint.h>

#define GRX_DISC_SECTOR 2048

/* ---- ISO 9660 image reader (disc.c) ------------------------------------ */

typedef struct GrxDisc GrxDisc;

/* A file or directory found in the image */
typedef struct GrxDiscEntry {
    uint32_t lba;          /* first sector of the extent */
    uint32_t size;         /* bytes */
    uint8_t flags;         /* ISO 9660 file flags: 2 = directory */
    uint8_t is_dir;
    char name[32];         /* as on the disc, without ";1" (and a trailing '.') */
} GrxDiscEntry;

/* Opens an image. Plain 2048-byte-sector images (.ISO, a DVD's .MDF) are
   what the game's disc gives; raw 2352-byte (and 2448 with subchannels)
   sectors, MODE1 or MODE2/XA form 1, are recognised by where the volume
   descriptor's "CD001" is. An .MDS path opens the .MDF beside it. Returns
   NULL (and a message in grx_disc_error()) when the file isn't an ISO 9660
   image. */
GrxDisc *grx_disc_open(const char *path);
void grx_disc_close(GrxDisc *d);
const char *grx_disc_error(void);

uint32_t grx_disc_sector_count(const GrxDisc *d);     /* volume space size */
void grx_disc_volume_id(const GrxDisc *d, char out[33]);
/* Sector layout found by grx_disc_open: bytes per sector and where the
   2048 bytes of user data start in each */
void grx_disc_layout(const GrxDisc *d, int *sector_size, int *data_offset);

/* Looks a path up: '/' or '\\' separators, any "cdrom0:" style device
   prefix and leading separators ignored, case-insensitive, ";1" versions
   optional. "" or "/" is the root directory. Returns 0, or -1 when there
   is no such file. */
int grx_disc_find(GrxDisc *d, const char *path, GrxDiscEntry *out);

/* Calls fn for each entry of a directory (not "." and ".."), in disc
   order; stops early when fn returns nonzero. Returns the number of
   entries visited, or -1 when the path isn't a directory. */
int grx_disc_list(GrxDisc *d, const char *dir,
                  int (*fn)(const GrxDiscEntry *e, void *user), void *user);

/* Reads count 2048-byte sectors from lba into dst. Sectors past the end of
   the image read as zeros (as far as the game is concerned they exist:
   files are rounded up to whole sectors). Returns count, or -1 on an I/O
   error. */
int grx_disc_read_sectors(GrxDisc *d, uint32_t lba, uint32_t count, void *dst);

/* Reads len bytes at offset into a file, clamped to its size. Returns the
   bytes read, or -1 on an I/O error. */
int64_t grx_disc_read_file(GrxDisc *d, const GrxDiscEntry *f, uint64_t offset,
                           void *dst, uint32_t len);

/* ---- The game's disc drivers (cdvd.c) ---------------------------------- */

/* The disc the drivers read from (owned by the caller) */
void grx_cdvd_set_disc(GrxDisc *d);
GrxDisc *grx_cdvd_disc(void);

/*
 * GS_IOP.IRX's stream side, emulated on the EE (the IOP half of SndStrm*,
 * SndFs* in src/main/45F610.c-4609A0.c). The game's C stays as it is: it
 * queues commands 0x100-0x1FF, sees the four stream rings in the IOP
 * status block and fetches ring sectors with sceSifGetOtherData. The rings
 * are virtual: a fetch reads the requested sectors straight from the disc.
 */

/* Fills the header GS_IOP.IRX writes to EE 0x01FF0000 once it is loaded
   (LoadIopModules 45EEB8 polls it for 0x5A5AA5A5, then takes the status
   and command buffer IOP addresses from it). Called from GS_IOP's RPC
   server (0x415249, port/src/sif.c), as on the PS2. */
void grx_gsiop_boot(void *ee_header);

/* A SIF DMA from the EE to an IOP address. Returns 1 when it was GS_IOP's
   command buffer (and has been processed), else 0. */
int grx_gsiop_dma(uint32_t iop_addr, const void *src, uint32_t size);

/* An IOP-to-EE copy (what sceSifGetOtherData does): the status block or
   ring sectors. Returns 1 when the address was GS_IOP's, else 0. */
int grx_gsiop_read(uint32_t iop_addr, void *dst, uint32_t size);

/* One queued GS_IOP command (SndIopCmd) */
void grx_gsiop_command(uint16_t seq, int16_t cmd, int32_t a0, int32_t a1, int32_t a2);

/* For the sound and movie driver emulation: takes up to n sectors from the
   front of a stream ring (as the IOP does for 0x20A, an SE bank from an
   open stream), copying them to dst when it isn't NULL. Returns the
   sectors taken. */
int grx_gsiop_strm_take(int slot, int n, void *dst);

/* The IOP status block as the IOP sees it (0x100 bytes, SndIopStatus) */
void *grx_gsiop_status(void);

#endif
