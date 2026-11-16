/* The freeze tool (port/docs/debug.md): freeze the game, keep its near
   past, and look at its state and memory.

   F9 freezes the game between two fields (and resumes it); F10, frozen,
   runs one more field. While it runs, the port keeps the last pictures,
   pad inputs and log lines; a freeze writes them, with the game's memory,
   to a dump directory (GRX_FREEZE_DIR, default build/freeze). A small
   server on 127.0.0.1 (GRX_DEBUG_PORT, default 7790; started at the first
   freeze, or at boot when GRX_DEBUG_PORT is set) answers
   port/tools/grxdbg.py: status, memory reads and writes, freeze, step,
   resume, dump. GRX_FREEZE_AT=field freezes at that field. */
#ifndef GRX_DEBUG_H
#define GRX_DEBUG_H

#include <stdint.h>

struct GrxVideoFrame;

/* Once the window and the GS are up (main.c) */
void grx_dbg_init(void);

/* A key from the window (SDL keycode): 1 if it was the tool's */
int grx_dbg_key(unsigned key);

/* The frame loop, before running a field: 1 while it must not run (frozen,
   no step left). Then the loop keeps the window alive and calls
   grx_dbg_poll() until it is 0. */
int grx_dbg_holding(void);

/* 1 while frozen (stepping included): the hang watchdog's exception;
   any thread */
int grx_dbg_frozen(void);

/* After each field has run (its number), on the game thread: history,
   GRX_FREEZE_AT, steps, and the server's requests */
void grx_dbg_field(uint64_t field);

/* The server's requests (on the game thread), also while frozen */
void grx_dbg_poll(void);

/* A picture the GPU renderer submits, on its thread with its context
   current */
void grx_dbg_picture_gl(const struct GrxVideoFrame *f);

/* Each log line (main.c's grx_log), any thread */
void grx_dbg_log_line(const char *line);

#endif
