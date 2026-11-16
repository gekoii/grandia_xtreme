/* The Display page (port/src/video_opts.c, port/game/field/62B3F8.c),
   kept in grandia.ini's [video] section:

   - Resolution: how sharp the GPU renderer draws (the window stays as it
     is sized, and the picture is scaled to it). Flexible, the default:
     the sharpest whole multiple of the PS2's 448 lines that fills the
     window, following it. 640x448: the PS2's own. The others: the whole
     multiple that reaches that many lines. GRX_GS_SCALE overrides it; the
     software renderer draws at the PS2's size whatever it says.
   - Frame rate: 60 (the default), a picture a field, or 30, every other
     one. The game runs 60 fields a second either way.
   - Aspect ratio: Auto (the default: the window's shape), or a fixed one
     (grx_widescreen.h); --aspect / GRX_ASPECT win over grandia.ini's. */
#ifndef GRX_VIDEO_OPTS_H
#define GRX_VIDEO_OPTS_H

enum {
    GRX_RES_FLEXIBLE, GRX_RES_ORIGINAL, GRX_RES_720, GRX_RES_1080, GRX_RES_1440, GRX_RES_2160,
    GRX_RES_COUNT
};

int grx_opts_resolution(void);
void grx_opts_set_resolution(int r); /* saves grandia.ini */
const char *grx_opts_resolution_name(int r);
/* The lines the setting asks for: 0 for Flexible (the window's) */
int grx_opts_resolution_lines(int r);

/* 30 or 60; any thread may read it */
int grx_opts_frame_rate(void);
void grx_opts_set_frame_rate(int fps); /* saves grandia.ini */

/* Aspect ratio: Auto, 4:3, 16:9, 16:10, 21:9, 32:9. grx_opts_aspect is
   the one in use (one given with --aspect: the nearest of these) */
#define GRX_ASPECT_COUNT 6
int grx_opts_aspect(void);
void grx_opts_set_aspect(int i); /* saves grandia.ini */
const char *grx_opts_aspect_name(int i);

/* grandia.ini (port/src/input.c reads and writes the file): the [video]
   section's lines (to a FILE *: the game's C includes this, without
   stdio.h), and one key of it; 0 if the key isn't the section's */
void grx_opts_ini_write(void *file);
int grx_opts_ini_read(const char *key, const char *value);

#endif
