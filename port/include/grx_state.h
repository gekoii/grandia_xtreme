/* Save states (port/src/state.c, port/docs/savestates.md): the whole
   game, saved to a file between two fields and loaded back, also in a
   later run of the same exe.

   The state is the game's memory (its RAM blocks, the hardware windows,
   its data in the exe), the stack of the game's main thread from where it
   waits for the next field up to GameMain, and what each part of the
   platform layer that stands in for the PS2's hardware keeps (the
   kernel's threads, GS memory, the sound chip, the IOP's streams, ...):
   each such part has a function that passes its state through
   grx_state_io, and is listed in state.c. */
#ifndef GRX_STATE_H
#define GRX_STATE_H

#include <stddef.h>
#include <stdint.h>

typedef struct GrxState GrxState;

/* Saving: copies n bytes from p into the state. Loading: copies them from
   the state to p. The same calls in the same order both ways. */
void grx_state_io(GrxState *s, void *p, size_t n);
#define GRX_STATE_VAR(s, v) grx_state_io((s), &(v), sizeof(v))

/* 1 while loading */
int grx_state_loading(const GrxState *s);

/* The keys (video.c): save to or load from slot 1-4. Carried out at the
   next field boundary where the game can be saved. */
void grx_state_request(int slot, int load);

/* From the kernel's idle hook (video.c), with every game thread waiting:
   carries out a pending request. 1 if it did (the hook then returns at
   once: a load has replaced its locals). */
int grx_state_service(void);

/* main.c: runs the game's entry; the saved stack is everything below */
int grx_state_run_game(int (*entry)(int, char **), int argc, char **argv);

/* A line for the window title while it is fresh (video.c), else NULL */
const char *grx_state_message(void);

#endif
