/* Widescreen: the aspect setting and where it is applied
   (grx_widescreen.h, port/docs/widescreen.md). */
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grx_widescreen.h"
#include "grx_video.h"
#include "grx_platform.h"
#include "gs/gs_internal.h"

#define ASPECT_MIN (4.0f / 3.0f)
#define ASPECT_MAX (32.0f / 9.0f)

static int ws_auto = 1;           /* follow the window */
static float ws_fixed = 16.0f / 9.0f; /* the aspect when not auto */
static float ws_now = 4.0f / 3.0f;
static int ws_extra;
static int ws_side;
static float ws_hud = 21.0f / 9.0f; /* the HUD at the edges up to this aspect */

static float clamp_aspect(float a) {
    return a < ASPECT_MIN ? ASPECT_MIN : a > ASPECT_MAX ? ASPECT_MAX : a;
}

/* W:H (or WxH, W/H) or a number: 0 if it is neither */
static float parse_aspect(const char *arg) {
    float w, h;
    char c;
    if (sscanf(arg, "%f%c%f", &w, &c, &h) == 3 && (c == ':' || c == 'x' || c == '/') && w > 0 && h > 0) {
        return w / h;
    }
    if (sscanf(arg, "%f", &w) == 1 && w > 0) {
        return w;
    }
    return 0;
}

/* --aspect or GRX_ASPECT was given: grandia.ini's setting gives way to it */
static int ws_from_command_line;

void grx_ws_config(const char *arg) {
    if (arg == NULL || !*arg) {
        arg = getenv("GRX_ASPECT");
    }
    ws_from_command_line = arg != NULL && *arg;
    if (arg == NULL || !*arg || strcmp(arg, "auto") == 0) {
        ws_auto = 1;
        return;
    }
    ws_fixed = parse_aspect(arg);
    if (ws_fixed == 0) {
        grx_fatal("--aspect / GRX_ASPECT: 4:3, 16:9, W:H or auto, not \"%s\"", arg);
    }
    ws_auto = 0;
    if (ws_fixed < ASPECT_MIN || ws_fixed > ASPECT_MAX) {
        grx_log("aspect %s is outside 4:3 to 32:9: clamped", arg);
    }
    ws_fixed = clamp_aspect(ws_fixed);
}

void grx_ws_set_aspect(float a, int from_ini) {
    if (from_ini && ws_from_command_line) {
        return;
    }
    if (a <= 0.0f) {
        ws_auto = 1;
    } else {
        ws_auto = 0;
        ws_fixed = clamp_aspect(a);
    }
}

float grx_ws_setting(void) {
    return ws_auto ? 0.0f : ws_fixed;
}

void grx_ws_hud_config(const char *arg) {
    if (arg == NULL || !*arg) {
        arg = getenv("GRX_HUD");
    }
    if (arg == NULL || !*arg) {
        ws_hud = 21.0f / 9.0f;
    } else if (strcmp(arg, "none") == 0 || strcmp(arg, "edges") == 0) {
        ws_hud = ASPECT_MAX;
    } else if (strcmp(arg, "centred") == 0 || strcmp(arg, "centered") == 0) {
        ws_hud = ASPECT_MIN;
    } else if ((ws_hud = parse_aspect(arg)) == 0) {
        grx_fatal("--hud / GRX_HUD: 4:3, 16:9, 21:9, W:H or none, not \"%s\"", arg);
    }
    ws_hud = clamp_aspect(ws_hud);
}

int grx_ws_window_width(int h) {
    float a = ws_auto ? 16.0f / 9.0f : ws_fixed;
    return (int)lrintf((float)h * a);
}

static void apply(float a) {
    float k;
    a = clamp_aspect(a);
    /* Quantise, so a window resized by a pixel doesn't change the frame */
    a = floorf(a * 1000.0f + 0.5f) / 1000.0f;
    if (a < ASPECT_MIN + 0.002f) {
        a = ASPECT_MIN;
    }
    if (a == ws_now && (gs_ws_on || a == ASPECT_MIN)) {
        return;
    }
    ws_now = a;
    k = ASPECT_MIN / a;
    /* The view's extra width on each side of the 640 pixels, whole pixels
       so the game's centre arithmetic stays exact */
    ws_extra = a == ASPECT_MIN ? 0 : (int)lrintf(320.0f / k - 320.0f);
    /* The HUD's: the same, for the aspect up to the --hud limit */
    if (a <= ws_hud) {
        ws_side = ws_extra;
    } else {
        ws_side = ws_hud <= ASPECT_MIN + 0.002f ? 0 : (int)lrintf(320.0f / (ASPECT_MIN / ws_hud) - 320.0f);
    }
    {
        /* GRX_WS_CULL=0: keep the 4:3 culling (to compare) */
        const char *e = getenv("GRX_WS_CULL");
        if (e != NULL && atoi(e) == 0) {
            ws_extra = 0;
        }
    }
    gs_ws_set(ws_auto || ws_fixed > ASPECT_MIN, a);
}

/* The game's screen capture (SysMain's GphFbStoreImage into
   sysState.screenCaptureAddr: 640x448 32-bit pixels, the frame as the GS
   holds it), which the menus draw behind them (GphScreenFxDraw's
   GphFbOverlayImage) for as long as they are open. It holds the picture
   squeezed for the aspect of its time; at another aspect the menu would
   draw it at the wrong width (the Display page's Aspect ratio changes it
   with a menu open). So when the aspect changes, it is resampled across
   to the new one: a column x now shows what was at 320 + (x - 320) * new
   / old, black where that is outside the picture. */
static uint32_t *capture;
static float capture_aspect;

/* From SysMain, after the capture (port/tools/fixups/widescreen.py) */
void grx_ws_capture_taken(unsigned addr);
void grx_ws_capture_taken(unsigned addr) {
    capture = (uint32_t *)(uintptr_t)addr;
    capture_aspect = ws_now;
}

static void resample_capture(void) {
    static uint32_t row[640];
    float k;
    int x, y, sx;
    if (capture == NULL || capture_aspect == ws_now) {
        return;
    }
    k = ws_now / capture_aspect;
    for (y = 0; y < 448; y++) {
        uint32_t *p = capture + y * 640;
        memcpy(row, p, sizeof(row));
        for (x = 0; x < 640; x++) {
            sx = (int)floorf(320.0f + ((float)x + 0.5f - 320.0f) * k);
            p[x] = sx >= 0 && sx < 640 ? row[sx] : 0x80000000u;
        }
    }
    capture_aspect = ws_now;
}

void grx_ws_update(void) {
    float a;
    if (!ws_auto) {
        a = ws_fixed;
    } else {
        int w = 0, h = 0;
        grx_video_window_size(&w, &h);
        a = w > 0 && h > 0 ? (float)w / (float)h : 16.0f / 9.0f;
    }
    if (a != ws_now || !gs_ws_on) {
        float before = ws_now;
        apply(a);
        if (ws_now != before) {
            grx_log("aspect: %.3f%s", ws_now, ws_auto ? " (auto)" : "");
            resample_capture();
        }
    }
}

float grx_ws_aspect(void) {
    return ws_now;
}

int grx_ws_view_extra(void) {
    return ws_extra;
}

int grx_ws_hud_side(void) {
    return ws_side;
}

/* HUD positions the game keeps from frame to frame were placed with the
   side of their time; the fixups move them by its change since, each
   frame, so they follow the window. i names a set of positions (port/
   tools/fixups/widescreen.py). */
int grx_ws_hud_moved(int i) {
    static int placed[4];
    int d = ws_side - placed[i];
    placed[i] = ws_side;
    return d;
}

/* VuViewClipSetup's view area (x, y, w, h, centerX, centerY, magH, magV,
   projDist, near, far, ...; port/tools/fixups/widescreen.py): a full-width
   view widened on both sides, its centre (x + centerX) kept */
const float *grx_ws_view(const float *c, float *tmp) {
    float d = (float)ws_extra;
    if (ws_extra == 0 || c[0] != 0.0f || c[2] != 640.0f) {
        return c;
    }
    memcpy(tmp, c, 12 * sizeof(float));
    tmp[0] = c[0] - d;
    tmp[2] = c[2] + 2.0f * d;
    tmp[4] = c[4] + d;
    return tmp;
}
