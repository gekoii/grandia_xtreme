/* The Display page's settings (grx_video_opts.h): the GPU renderer's
   resolution (gs_gl/gsgl.c applies it at a vsync), the frame rate
   (video.c) and the aspect ratio (widescreen.c). */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "grx_video_opts.h"
#include "grx_input.h"
#include "grx_platform.h"
#include "grx_widescreen.h"

static const struct {
    const char *name; /* on the page and in grandia.ini */
    int lines;
} res[GRX_RES_COUNT] = {
    {"Flexible", 0}, {"640x448", 448}, {"1280x720", 720}, {"1920x1080", 1080},
    {"2560x1440", 1440}, {"3840x2160", 2160},
};

static const struct {
    const char *name;
    float aspect; /* 0: auto */
} aspects[GRX_ASPECT_COUNT] = {
    {"Auto", 0.0f}, {"4:3", 4.0f / 3.0f}, {"16:9", 16.0f / 9.0f}, {"16:10", 16.0f / 10.0f},
    {"21:9", 21.0f / 9.0f}, {"32:9", 32.0f / 9.0f},
};

static int resolution = GRX_RES_FLEXIBLE;
static SDL_AtomicInt frame_rate = {60};
static int ini_aspect; /* the page's or grandia.ini's (--aspect isn't saved) */

static void changed(const char *what, const char *value) {
    grx_log("video: %s %s", what, value);
    grx_settings_save();
}

int grx_opts_resolution(void) {
    return resolution;
}

void grx_opts_set_resolution(int r) {
    if (r < 0 || r >= GRX_RES_COUNT || r == resolution) {
        return;
    }
    resolution = r;
    changed("resolution", res[r].name);
}

const char *grx_opts_resolution_name(int r) {
    return r >= 0 && r < GRX_RES_COUNT ? res[r].name : "";
}

int grx_opts_resolution_lines(int r) {
    return r >= 0 && r < GRX_RES_COUNT ? res[r].lines : 0;
}

int grx_opts_frame_rate(void) {
    return SDL_GetAtomicInt(&frame_rate);
}

void grx_opts_set_frame_rate(int fps) {
    fps = fps == 30 ? 30 : 60;
    if (fps == SDL_GetAtomicInt(&frame_rate)) {
        return;
    }
    SDL_SetAtomicInt(&frame_rate, fps);
    changed("frame rate", fps == 30 ? "30" : "60");
}

int grx_opts_aspect(void) {
    float a = grx_ws_setting();
    int i, best = 0;
    if (a <= 0.0f) {
        return 0;
    }
    for (i = 1; i < GRX_ASPECT_COUNT; i++) {
        if (best == 0 || fabsf(aspects[i].aspect - a) < fabsf(aspects[best].aspect - a)) {
            best = i;
        }
    }
    return best;
}

void grx_opts_set_aspect(int i) {
    if (i < 0 || i >= GRX_ASPECT_COUNT || i == grx_opts_aspect()) {
        return;
    }
    ini_aspect = i;
    grx_ws_set_aspect(aspects[i].aspect, 0);
    changed("aspect ratio", aspects[i].name);
}

const char *grx_opts_aspect_name(int i) {
    return i >= 0 && i < GRX_ASPECT_COUNT ? aspects[i].name : "";
}

void grx_opts_ini_write(void *file) {
    FILE *f = file;
    if (resolution == GRX_RES_FLEXIBLE && grx_opts_frame_rate() == 60 && ini_aspect == 0) {
        return;
    }
    fprintf(f, "\n[video]\nresolution = %s\nframe_rate = %d\naspect_ratio = %s\n", res[resolution].name,
            grx_opts_frame_rate(), aspects[ini_aspect].name);
}

int grx_opts_ini_read(const char *key, const char *value) {
    int r;
    if (SDL_strcasecmp(key, "resolution") == 0) {
        for (r = 0; r < GRX_RES_COUNT; r++) {
            if (SDL_strcasecmp(value, res[r].name) == 0) {
                resolution = r;
                return 1;
            }
        }
        /* (640x448 was called Original at first) */
        if (SDL_strcasecmp(value, "Original") == 0) {
            resolution = GRX_RES_ORIGINAL;
            return 1;
        }
        return 0;
    }
    if (SDL_strcasecmp(key, "frame_rate") == 0) {
        r = SDL_atoi(value);
        if (r != 30 && r != 60) {
            return 0;
        }
        SDL_SetAtomicInt(&frame_rate, r);
        return 1;
    }
    if (SDL_strcasecmp(key, "aspect_ratio") == 0) {
        for (r = 0; r < GRX_ASPECT_COUNT; r++) {
            if (SDL_strcasecmp(value, aspects[r].name) == 0) {
                ini_aspect = r;
                grx_ws_set_aspect(aspects[r].aspect, 1);
                return 1;
            }
        }
        return 0;
    }
    return 0;
}
