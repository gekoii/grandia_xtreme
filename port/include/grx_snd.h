#ifndef GRX_SND_H
#define GRX_SND_H

/*
 * GS_IOP.IRX's sound driver (its spu2_command / spu2_server), rewritten
 * on top of the software SPU2 (grx_spu2.h). port/docs/sound.md has the
 * command set, the formats and how the original works.
 *
 * Threads: the commands and grx_snd_poll come from the game's thread (via
 * port/src/cdvd.c), grx_snd_render from the audio device's thread. Every
 * entry point but grx_snd_render takes the lock installed with
 * grx_snd_set_lock (audio.c installs the SDL audio stream's); the audio
 * callback already holds it when it calls grx_snd_render. Disc reads
 * happen only on the game's thread, outside the lock.
 */

#include <stdint.h>
#include "grx_spu2.h"

/* Creates the SPU2 (idempotent). The driver itself starts with command
   0x200, as on the IOP. Returns 0, or -1 when out of memory. */
int grx_snd_init(void);
void grx_snd_quit(void);

/* One GS_IOP command from the EE's queue (SndIopCmd, include/main/n10.h):
   cmd as queued (0x200-0x2FF are the sound driver's), args its three
   int32 arguments (size 12). Returns 1 when the command was the sound
   driver's (handled, or known and ignored), 0 when it isn't one. May read
   the disc (0x208, 0x20A). */
int grx_snd_command(uint32_t cmd, const void *args, uint32_t size);

/* From the game's thread whenever the EE reads the IOP status block
   (SndIopPoll): tops up the music streams from the disc and writes the
   driver's fields into the status block (bankState, strmInfo, ...). With
   no audio device (grx_snd_set_realtime(0), the default) it also runs the
   driver for the time that has passed, so streams still progress. */
void grx_snd_poll(void);

/* The audio thread: runs the driver and the SPU2 for `frames` 48 kHz
   stereo frames (interleaved s16). The caller holds the lock. */
void grx_snd_render(int16_t *out, int frames);

/* The lock around the driver's state (default: none) */
void grx_snd_set_lock(void (*lock)(void *), void (*unlock)(void *), void *ctx);

/* 1 while an audio device calls grx_snd_render; 0 lets grx_snd_poll keep
   time with the wall clock instead */
void grx_snd_set_realtime(int on);

/* For the movie player's emulation (0x300, not done here): appends n
   2048-byte sectors of a VBS stream to stream `stream` (0/1) when it was
   prepared from no disc data (0x208 with 0 sectors). Returns the sectors
   taken. */
int grx_snd_strm_feed(int stream, const void *sectors, int n);

/* The SPU2 behind the driver (tests, debugging) */
GrxSpu2 *grx_snd_spu2(void);

#endif
