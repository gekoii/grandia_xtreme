/* Widescreen (port/docs/widescreen.md): --aspect 4:3|16:9|W:H|auto, or
   GRX_ASPECT. The game keeps drawing into its 640x448 frame buffers; at a
   wider aspect the GS front end squeezes the picture horizontally into
   them (port/src/gs/gs_widescreen.c) and the presenter stretches it back,
   the game's view culling is widened to match (port/tools/fixups/
   widescreen.py), and the GPU renderer draws at a larger horizontal
   internal scale. At 4:3 nothing changes. */
#ifndef GRX_WIDESCREEN_H
#define GRX_WIDESCREEN_H

/* The --aspect / GRX_ASPECT setting (NULL: GRX_ASPECT, else auto). Fatal
   on a bad value. Call before grx_video_init. */
void grx_ws_config(const char *arg);

/* The Display page's Aspect ratio (grx_video_opts.h): a W/H, or 0 for
   auto, from the next field. From grandia.ini (from_ini) it gives way to
   --aspect / GRX_ASPECT; from the page it doesn't */
void grx_ws_set_aspect(float a, int from_ini);

/* The aspect asked for: a W/H, or 0 for auto */
float grx_ws_setting(void);

/* The --hud / GRX_HUD setting (NULL: GRX_HUD, else 21:9): the widest
   aspect the HUD follows to the screen's sides, 4:3 to keep it where it is
   at 4:3, W:H, or none for no limit. Wider than the limit, it stays where
   it is at the limit, the picture going on past it. Fatal on a bad
   value. */
void grx_ws_hud_config(const char *arg);

/* The window's width for a height of h lines: 4:3 when the aspect is
   fixed at 4:3, else the aspect's (16:9 for auto) */
int grx_ws_window_width(int h);

/* Once the window exists (before the renderer starts), and once a field:
   in auto mode, follow the window's (or screen's) shape. Changes take
   effect from the next field. */
void grx_ws_update(void);

/* The picture's display aspect now (4/3 .. 32/9) */
float grx_ws_aspect(void);

/* For the game's view set-up (VuViewClipSetup, through the fixups): how
   many pixels the full-width 640-pixel view is widened by on each side,
   in the game's 640-wide screen space (0 at 4:3) */
int grx_ws_view_extra(void);

/* For the HUD anchored to the screen's sides (the field's mini-map and
   threat gauge, through the fixups): how far the screen's edges are past
   x = 0 and x = 640, in the game's screen space (0 at 4:3; for the aspect up
   to the --hud limit; not changed by GRX_WS_CULL) */
int grx_ws_hud_side(void);

/* VuViewClipSetup's view (from &gphViews[i].x): c itself, or tmp filled
   with the view widened by grx_ws_view_extra() on both sides when it is
   full width */
const float *grx_ws_view(const float *c, float *tmp);

#endif
