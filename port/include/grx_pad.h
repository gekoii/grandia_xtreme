/* The controllers (port/src/pad.c): libpad (scePad*) over SDL3 gamepads
   and the keyboard. The scePad* calls have the game's names and are not
   declared here.

   Port 0 is the keyboard plus the first gamepad, port 1 the second
   gamepad (disconnected without one). Each looks like a DualShock 2 to the
   game: it starts in digital mode (ID 0x41), and the game's own requests
   switch it to analog (0x73) and pressure mode (0x79), each taking a few
   fields as padman's do.

   Which key and gamepad input is which button: grx_input.h, set on the
   Configurations page. The keyboard's defaults: arrows = d-pad, Z = cross,
   X = circle, A = square, S = triangle, Q = L1, W = R1, E = L2, R = R2,
   Enter = start, Backspace = select. */
#ifndef GRX_PAD_H
#define GRX_PAD_H

union SDL_Event;

/* After SDL_Init(SDL_INIT_GAMEPAD): opens the gamepads already connected */
void grx_pad_init(void);
void grx_pad_shutdown(void);

/* Gamepads added and removed (from grx_video_pump) */
void grx_pad_event(const union SDL_Event *ev);

/* Once per field, as padman's update: samples the input into each open
   port's pad data and advances its pending requests */
void grx_pad_update(void);

/* One line describing a port (state, mode, the data bytes), for --pad-test */
void grx_pad_describe(int port, char *buf, int n);

/* A port's input now, for the freeze tool's history: the buttons (bit 0
   L2 ... 7 square, 8 select ... 15 left; 1 = down) in bits 0-15, the left
   stick's X and Y (0x80 centre) in bits 16-23 and 24-31 */
unsigned grx_pad_raw(int port);

/* A port's gamepad: 1 an Xbox (or generic XInput) one, 0 another kind
   (PlayStation, Nintendo), -1 none */
int grx_pad_xbox_like(int port);

/* 1 while port 1 is played on the keyboard: it has no gamepad, or a key
   was the last thing pressed (a gamepad's button, trigger or stick since
   makes it 0) */
int grx_pad_keyboard(void);

/* A press for GRX_PAD_SCRIPT's list, on port 1: `name` as there (up,
   circle, l1, start, rright, ...), from `field` for `hold` fields. 0, or
   -1 for an unknown name or a full list. */
int grx_pad_script_add(const char *name, unsigned long long field, unsigned hold);

#endif
