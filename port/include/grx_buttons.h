/* Button prompts and layout for the controller in use (port/docs/xbox.md):
   --buttons auto|xbox|playstation, or GRX_BUTTONS. With an Xbox
   controller (auto: when port 1's gamepad is an Xbox or a generic XInput
   one) A confirms and B cancels, as on Xbox, and the game's button icons
   show A, B, X, Y, LB, RB, LT and RT. */
#ifndef GRX_BUTTONS_H
#define GRX_BUTTONS_H

#include <stdint.h>

/* The --buttons / GRX_BUTTONS setting (NULL: GRX_BUTTONS, else auto).
   Fatal on a bad value. */
void grx_buttons_config(const char *arg);

/* 1 while the Xbox layout is in use (pad.c swaps A and B by it) */
int grx_buttons_xbox(void);

/* Once a field, after the pads: follow the controller, and keep the
   game's icon textures showing its buttons */
void grx_buttons_field(void);

/* How far right the title's prompt goes (0, or 55 while it says PRESS
   ENTER or PRESS START, on the keyboard, without BUTTON to centre it on) */
int grx_buttons_prompt_shift(void);

/* A button's icon in the icons texture (id 0xEC) as the layout shows it
   (the game's prompts): i 0-7 the circle, cross, square, triangle, L1,
   R1, L2 and R2 icons, which the Xbox layout shows as A, B, X, Y, LB, RB,
   LT and RT. Its top-left texel and width (32 high); 0 while the texture
   isn't known. */
int grx_buttons_icon(int i, int *u, int *v, int *w);

/* A half-size PlayStation button in the icons texture (id 0xEC), in either
   layout, for the port's own lists: i 0-7 circle, cross, square, triangle,
   L1, R1, L2, R2. Its top-left texel and width (16 high); 0 while the
   texture isn't known. */
int grx_buttons_ps_icon(int i, int *u, int *v, int *w);

/* Redraw a texture's PlayStation buttons as Xbox ones, in place: `px` its
   256 x 256 indices, `clut` its palette (CSM1 order) */
enum { GRX_BUTTONS_ICONS, GRX_BUTTONS_HUD };
void grx_buttons_make_xbox(int which, uint8_t *px, const uint32_t *clut);

#endif
