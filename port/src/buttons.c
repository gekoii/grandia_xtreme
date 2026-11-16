/* Button prompts for the controller in use (grx_buttons.h,
   port/docs/xbox.md).

   The game draws its button prompts from two textures in SYS/GRXSYS.PAC:
   - id 0xEC (texE7[5]), the menu and battle icons: the L1, R1, L2 and R2
     plaques and the circle, triangle, cross and square buttons;
   - id 0xED (texE7[6]), the battle HUD: a small triangle button and the
     "L2 SKILL RETURN R2" label.
   Both are 256x256 8-bit textures that GphTexBind uploads from main
   memory every time they are bound. So for an Xbox controller the port
   redraws those parts in memory (the art is made here, from the game's
   own pixels: the PlayStation symbol is painted out and a letter drawn,
   in the texture's palette), and puts the originals back for a
   PlayStation one. The next bind shows it, with either renderer.

   Each texture is recognised by a hash of its pixels and palette, so a
   different release, or a texture this doesn't know, is left alone. A
   reload of the pack (the regions no longer hold what was written) is
   noticed and handled the same way. */
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "grx_buttons.h"
#include "grx_platform.h"

/* ---------------------------------------------------------------------- */
/* The textures */

#define TEX_W 256
#define TEX_PIXELS (TEX_W * TEX_W)

/* A palette index's CLUT entry: CSM1 order swaps 8-15 and 16-23 of each 32 */
static int clut_slot(int i) {
    int b = i & 0x18;
    return (i & ~0x18) | (b == 8 ? 16 : b == 16 ? 8 : b);
}

typedef struct {
    uint8_t *px;         /* 256 x 256 indices */
    const uint32_t *clut; /* 256 entries, 0xAABBGGRR, CSM1 order */
    uint8_t rgb[256][4]; /* by index: r, g, b, a */
    int16_t cache[32768]; /* nearest opaque index for a 15-bit colour, -1 unknown */
} Tex;

static void tex_init(Tex *t, uint8_t *px, const uint32_t *clut) {
    int i;
    t->px = px;
    t->clut = clut;
    for (i = 0; i < 256; i++) {
        uint32_t c = clut[clut_slot(i)];
        t->rgb[i][0] = (uint8_t)c;
        t->rgb[i][1] = (uint8_t)(c >> 8);
        t->rgb[i][2] = (uint8_t)(c >> 16);
        t->rgb[i][3] = (uint8_t)(c >> 24);
    }
    for (i = 0; i < 32768; i++) {
        t->cache[i] = -1;
    }
}

/* The opaque palette entry nearest a colour */
static uint8_t nearest(Tex *t, float r, float g, float b) {
    int ir = (int)(r < 0 ? 0 : r > 255 ? 255 : r), ig = (int)(g < 0 ? 0 : g > 255 ? 255 : g),
        ib = (int)(b < 0 ? 0 : b > 255 ? 255 : b);
    int key = (ir >> 3) | (ig >> 3) << 5 | (ib >> 3) << 10, i, best = 0;
    long bd = 1L << 30;
    if (t->cache[key] >= 0) {
        return (uint8_t)t->cache[key];
    }
    for (i = 0; i < 256; i++) {
        long dr, dg, db, d;
        if (t->rgb[i][3] < 0x70) {
            continue;
        }
        dr = t->rgb[i][0] - ir;
        dg = t->rgb[i][1] - ig;
        db = t->rgb[i][2] - ib;
        d = 3 * dr * dr + 4 * dg * dg + 2 * db * db;
        if (d < bd) {
            bd = d;
            best = i;
        }
    }
    t->cache[key] = (int16_t)best;
    return (uint8_t)best;
}

#define PX(t, x, y) ((t)->px[(y) * TEX_W + (x)])
#define RGB(t, x, y) ((t)->rgb[PX(t, x, y)])

static int sat(const uint8_t *c) {
    int mx = c[0] > c[1] ? (c[0] > c[2] ? c[0] : c[2]) : (c[1] > c[2] ? c[1] : c[2]);
    int mn = c[0] < c[1] ? (c[0] < c[2] ? c[0] : c[2]) : (c[1] < c[2] ? c[1] : c[2]);
    return mx - mn;
}

/* ---------------------------------------------------------------------- */
/* Letters, as strokes (a distance field), for the face buttons */

typedef struct {
    float ax, ay, bx, by; /* a segment; an arc when r > 0: centre ax, ay, right half */
    float r;
} Stroke;

/* In a 32 x 32 cell, centre 16, 16 */
static const Stroke letter_a[] = {{10.6f, 23.4f, 16.0f, 8.6f, 0}, {16.0f, 8.6f, 21.4f, 23.4f, 0},
                                  {12.9f, 18.6f, 19.1f, 18.6f, 0}, {0, 0, 0, 0, -1}};
static const Stroke letter_b[] = {{11.4f, 8.6f, 11.4f, 23.4f, 0},  {11.4f, 8.6f, 16.2f, 8.6f, 0},
                                  {16.2f, 12.0f, 0, 0, 3.4f},       {11.4f, 15.4f, 16.6f, 15.4f, 0},
                                  {16.6f, 19.4f, 0, 0, 4.0f},       {11.4f, 23.4f, 16.6f, 23.4f, 0},
                                  {0, 0, 0, 0, -1}};
static const Stroke letter_x[] = {{11.0f, 8.6f, 21.0f, 23.4f, 0}, {21.0f, 8.6f, 11.0f, 23.4f, 0}, {0, 0, 0, 0, -1}};
static const Stroke letter_y[] = {{10.8f, 8.6f, 16.0f, 16.2f, 0}, {21.2f, 8.6f, 16.0f, 16.2f, 0},
                                  {16.0f, 16.2f, 16.0f, 23.4f, 0}, {0, 0, 0, 0, -1}};

static float seg_dist(float px, float py, const Stroke *s) {
    if (s->r > 0) {
        /* The right half of a ring */
        float dx = px - s->ax, dy = py - s->ay;
        if (dx >= 0) {
            return fabsf(sqrtf(dx * dx + dy * dy) - s->r);
        } else {
            float d1 = sqrtf(dx * dx + (dy + s->r) * (dy + s->r)), d2 = sqrtf(dx * dx + (dy - s->r) * (dy - s->r));
            return d1 < d2 ? d1 : d2;
        }
    } else {
        float vx = s->bx - s->ax, vy = s->by - s->ay, wx = px - s->ax, wy = py - s->ay;
        float l = vx * vx + vy * vy, k = l > 0 ? (wx * vx + wy * vy) / l : 0;
        if (k < 0) k = 0;
        if (k > 1) k = 1;
        wx -= k * vx;
        wy -= k * vy;
        return sqrtf(wx * wx + wy * wy);
    }
}

/* How much of a pixel (x, y in cell units, `step` wide) the letter covers */
static float coverage(const Stroke *l, float x, float y, float step, float half_width) {
    int sx, sy, in = 0;
    for (sy = 0; sy < 4; sy++) {
        for (sx = 0; sx < 4; sx++) {
            float px = x + (sx + 0.5f) * step / 4, py = y + (sy + 0.5f) * step / 4, d = 1e9f;
            const Stroke *s;
            for (s = l; s->r >= 0; s++) {
                float e = seg_dist(px, py, s);
                if (e < d) d = e;
            }
            in += d <= half_width;
        }
    }
    return in / 16.0f;
}

/* A round button: the symbol inside radius `inner` painted out (each row
   filled across from the grey on either side), then `letter` drawn in
   `colour`. The cell is size x size texels at x0, y0. */
static void face_button(Tex *t, int x0, int y0, int size, const Stroke *letter, const uint8_t colour[3]) {
    float c = size / 2.0f, inner = size * 0.40f, k = 32.0f / size;
    uint8_t sym[32][32];
    float bg[32][32][3];
    int x, y;
    memset(sym, 0, sizeof(sym));
    /* The symbol: saturated or bright pixels, grown by one */
    for (y = 0; y < size; y++) {
        for (x = 0; x < size; x++) {
            float dx = x + 0.5f - c, dy = y + 0.5f - c;
            const uint8_t *p = RGB(t, x0 + x, y0 + y);
            if (dx * dx + dy * dy <= inner * inner && (sat(p) > 24 || p[0] + p[1] + p[2] > 3 * 135)) {
                sym[y][x] = 1;
            }
        }
    }
    for (y = 0; y < size; y++) {
        for (x = 0; x < size; x++) {
            float dx = x + 0.5f - c, dy = y + 0.5f - c;
            int i, j, near = 0;
            for (j = -1; j <= 1; j++) {
                for (i = -1; i <= 1; i++) {
                    if (y + j >= 0 && y + j < size && x + i >= 0 && x + i < size && (sym[y + j][x + i] & 1)) {
                        near = 1;
                    }
                }
            }
            if (near && !(sym[y][x] & 1) && dx * dx + dy * dy <= (inner + 1) * (inner + 1)) {
                sym[y][x] = 2;
            }
        }
    }
    /* The grey behind it, across each row */
    for (y = 0; y < size; y++) {
        for (x = 0; x < size; x++) {
            int l = x, r = x, ch;
            const uint8_t *pl, *pr;
            float w;
            if (!sym[y][x]) {
                for (ch = 0; ch < 3; ch++) {
                    bg[y][x][ch] = RGB(t, x0 + x, y0 + y)[ch];
                }
                continue;
            }
            while (l > 0 && sym[y][l]) l--;
            while (r < size - 1 && sym[y][r]) r++;
            pl = RGB(t, x0 + l, y0 + y);
            pr = RGB(t, x0 + r, y0 + y);
            w = r > l ? (float)(x - l) / (float)(r - l) : 0.5f;
            for (ch = 0; ch < 3; ch++) {
                bg[y][x][ch] = pl[ch] + (pr[ch] - pl[ch]) * w;
            }
        }
    }
    /* The letter over it */
    for (y = 0; y < size; y++) {
        for (x = 0; x < size; x++) {
            float a = coverage(letter, x * k, y * k, k, 1.75f);
            float r, g, b;
            if (!sym[y][x] && a == 0) {
                continue;
            }
            r = bg[y][x][0] + (colour[0] - bg[y][x][0]) * a;
            g = bg[y][x][1] + (colour[1] - bg[y][x][1]) * a;
            b = bg[y][x][2] + (colour[2] - bg[y][x][2]) * a;
            PX(t, x0 + x, y0 + y) = nearest(t, r, g, b);
        }
    }
}

/* ---------------------------------------------------------------------- */
/* The shoulder plaques: the digit becomes B or T, in the plaque's own
   cream with a black outline */

static const char *const plaque_b[] = {
    "#########..", "##########.", ".#####.####", ".#####..###", ".#####..###", ".#####..###", ".#####..###",
    ".#####.####", ".#########.", ".########..", ".#########.", ".#####.####", ".#####..###", ".#####..###",
    ".#####..###", ".#####..###", ".#####..###", ".#####.####", "##########.", "#########..", NULL};
static const char *const plaque_t[] = {
    "###########", "###########", "##.#####.##", "#..#####..#", "...#####...", "...#####...", "...#####...",
    "...#####...", "...#####...", "...#####...", "...#####...", "...#####...", "...#####...", "...#####...",
    "...#####...", "...#####...", "...#####...", "...#####...", ".#########.", ".#########.", NULL};

static int is_cream(const uint8_t *p) {
    return p[3] >= 0x70 && p[0] > 200 && p[1] > 190 && p[2] > 180;
}
static int is_black(const uint8_t *p) {
    return p[3] >= 0x70 && p[0] < 40 && p[1] < 40 && p[2] < 40;
}

/* The plaque at x0, y0 (40 x 32) */
static void plaque(Tex *t, int x0, int y0, const char *const *glyph) {
    static uint8_t comp[32][40];
    int x, y, n = 0, digit = 0, maxx = -1, minx = 99, gx, gy, cream = -1, black = -1, i, j;
    /* Cream components (4-connected) */
    memset(comp, 0, sizeof(comp));
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 40; x++) {
            int sp, stx[1280], sty[1280], cmin = 99, size = 0;
            if (comp[y][x] || !is_cream(RGB(t, x0 + x, y0 + y)) || y < 3) {
                continue;
            }
            n++;
            sp = 0;
            stx[sp] = x;
            sty[sp++] = y;
            comp[y][x] = (uint8_t)n;
            while (sp > 0) {
                int cx = stx[--sp], cy = sty[sp], d;
                static const int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
                if (cx < cmin) cmin = cx;
                size++;
                for (d = 0; d < 4; d++) {
                    int nx = cx + dx[d], ny = cy + dy[d];
                    if (nx >= 0 && nx < 40 && ny >= 3 && ny < 32 && !comp[ny][nx] &&
                        is_cream(RGB(t, x0 + nx, y0 + ny)) && sp < 1280) {
                        comp[ny][nx] = (uint8_t)n;
                        stx[sp] = nx;
                        sty[sp++] = ny;
                    }
                }
            }
            /* A letter, not the frame's thin highlights: the digit is the
               letter that starts furthest right */
            if (size >= 40 && cmin > maxx) {
                maxx = cmin;
                digit = n;
            }
        }
    }
    if (digit == 0) {
        return;
    }
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 40; x++) {
            if (comp[y][x] == digit) {
                if (x < minx) minx = x;
                cream = PX(t, x0 + x, y0 + y);
            }
        }
    }
    /* Paint the digit and its own outline out with the brown to its right */
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 40; x++) {
            int mine = comp[y][x] == digit, edge = 0, other = 0;
            if (!mine && is_black(RGB(t, x0 + x, y0 + y))) {
                black = PX(t, x0 + x, y0 + y);
                for (j = -1; j <= 1; j++) {
                    for (i = -1; i <= 1; i++) {
                        int ny = y + j, nx = x + i;
                        if (ny >= 0 && ny < 32 && nx >= 0 && nx < 40 && comp[ny][nx]) {
                            if (comp[ny][nx] == digit) edge = 1;
                            else other = 1;
                        }
                    }
                }
            }
            if (mine || (edge && !other)) {
                int k, fill = -1;
                for (k = 1; k < 12 && fill < 0; k++) {
                    int xs[2] = {x + k, x - k};
                    for (i = 0; i < 2 && fill < 0; i++) {
                        const uint8_t *p;
                        if (xs[i] < 0 || xs[i] >= 40) continue;
                        p = RGB(t, x0 + xs[i], y0 + y);
                        if (comp[y][xs[i]] == 0 && !is_black(p) && !is_cream(p) && p[3] >= 0x70 && sat(p) > 20 &&
                            p[0] < 200) {
                            fill = PX(t, x0 + xs[i], y0 + y);
                        }
                    }
                }
                if (fill >= 0) {
                    PX(t, x0 + x, y0 + y) = (uint8_t)fill;
                }
            }
        }
    }
    if (cream < 0 || black < 0) {
        return;
    }
    /* The new letter, where the digit started, rows 5 to 24 */
    gx = minx;
    gy = 5;
    for (y = 0; glyph[y] != NULL; y++) {
        for (x = 0; glyph[y][x]; x++) {
            if (glyph[y][x] == '#') {
                PX(t, x0 + gx + x, y0 + gy + y) = (uint8_t)cream;
            }
        }
    }
    /* Its outline: what touches it, but not another letter's cream */
    {
        int rows = 0, w = (int)strlen(glyph[0]);
        while (glyph[rows]) rows++;
#define G(yy, xx) ((yy) >= 0 && (yy) < rows && (xx) >= 0 && (xx) < w && glyph[yy][xx] == '#')
        for (y = -1; y <= rows; y++) {
            for (x = -1; x <= w; x++) {
                int touch = 0;
                if (G(y, x)) continue;
                for (j = -1; j <= 1; j++) {
                    for (i = -1; i <= 1; i++) {
                        touch |= G(y + j, x + i);
                    }
                }
                if (touch && !is_cream(RGB(t, x0 + gx + x, y0 + gy + y))) {
                    PX(t, x0 + gx + x, y0 + gy + y) = (uint8_t)black;
                }
            }
        }
#undef G
    }
}

/* ---------------------------------------------------------------------- */
/* The HUD label's small "2"s become "T"s (9 x 9, each row in the digit's
   own colour, with the font's black shadow right and below) */

static const char *const small_t[] = {"#########", "#..###..#", "...###...", "...###...", "...###...",
                                      "...###...", "...###...", "...###...", "..#####..", NULL};

static void small_digit(Tex *t, int x0, int y0) {
    int row_colour[9], x, y, transparent = -1;
    uint8_t m[11][12];
    for (y = 0; y < 9; y++) {
        row_colour[y] = -1;
        for (x = 0; x < 10 && row_colour[y] < 0; x++) {
            const uint8_t *p = RGB(t, x0 + x, y0 + y);
            if (p[3] >= 0x70 && !is_black(p)) {
                row_colour[y] = PX(t, x0 + x, y0 + y);
            }
        }
        if (row_colour[y] < 0) {
            row_colour[y] = y > 0 ? row_colour[y - 1] : -1;
        }
    }
    for (x = 0; x < 256 && transparent < 0; x++) {
        if (t->rgb[x][3] == 0) transparent = x;
    }
    if (transparent < 0 || row_colour[0] < 0) {
        return;
    }
    memset(m, 0, sizeof(m));
    for (y = 0; y < 10; y++) {
        for (x = 0; x < 10; x++) {
            PX(t, x0 + x, y0 + y) = (uint8_t)transparent;
        }
    }
    for (y = 0; small_t[y]; y++) {
        for (x = 0; small_t[y][x]; x++) {
            if (small_t[y][x] == '#') {
                PX(t, x0 + x, y0 + y) = (uint8_t)row_colour[y];
                m[y][x] = 1;
            }
        }
    }
    for (y = 0; y < 10; y++) {
        for (x = 0; x < 10; x++) {
            if (!m[y][x] && ((x > 0 && m[y][x - 1]) || (y > 0 && m[y - 1][x]) || (x > 0 && y > 0 && m[y - 1][x - 1]))) {
                PX(t, x0 + x, y0 + y) = nearest(t, 0, 0, 0);
            }
        }
    }
}

/* The game draws the round buttons from one texel right of and below where
   their art starts (u 0x56 for a disc whose first column is 0x55; v 0xBE
   for 0xBD), so their left column and top row never show, and the column
   after them does. Move a 32 x 32 button there, so it shows whole. */
static void shift_button(Tex *t, int x0, int y0) {
    int x, y, clear = -1;
    for (x = 0; x < 256 && clear < 0; x++) {
        if (t->rgb[x][3] == 0) clear = x;
    }
    if (clear < 0) {
        return;
    }
    for (y = 31; y >= 0; y--) {
        for (x = 31; x >= 0; x--) {
            PX(t, x0 + x + 1, y0 + y + 1) = PX(t, x0 + x, y0 + y);
        }
    }
    for (x = 0; x < 33; x++) {
        PX(t, x0 + x, y0) = (uint8_t)clear;
    }
    for (y = 0; y < 33; y++) {
        PX(t, x0, y0 + y) = (uint8_t)clear;
    }
}

/* ---------------------------------------------------------------------- */

static const uint8_t c_a[3] = {96, 200, 64}, c_b[3] = {230, 64, 56}, c_x[3] = {56, 130, 236},
                     c_y[3] = {244, 200, 48};

void grx_buttons_make_xbox(int which, uint8_t *px, const uint32_t *clut) {
    static Tex t;
    tex_init(&t, px, clut);
    if (which == GRX_BUTTONS_ICONS) {
        face_button(&t, 85, 189, 32, letter_a, c_a); /* circle: confirm, A */
        face_button(&t, 119, 189, 32, letter_y, c_y); /* triangle: Y */
        face_button(&t, 85, 223, 32, letter_b, c_b); /* cross: B */
        face_button(&t, 119, 223, 32, letter_x, c_x); /* square: X */
        shift_button(&t, 85, 189);
        shift_button(&t, 119, 189);
        shift_button(&t, 85, 223);
        shift_button(&t, 119, 223);
        plaque(&t, 1, 189, plaque_b);   /* L1: LB */
        plaque(&t, 43, 189, plaque_b);  /* R1: RB */
        plaque(&t, 1, 223, plaque_t);   /* L2: LT */
        plaque(&t, 43, 223, plaque_t);  /* R2: RT */
    } else {
        face_button(&t, 236, 108, 16, letter_y, c_y); /* the HUD's triangle */
        small_digit(&t, 11, 245);  /* L2 */
        small_digit(&t, 152, 245); /* R2 */
    }
}

/* ---------------------------------------------------------------------- */
/* Following the controller */

#ifndef GRX_BUTTONS_ART_ONLY
#include <stdio.h>
#include <SDL3/SDL_scancode.h>
#include "grx_input.h"
#include "grx_pad.h"

/* GRXSYS.PAC once loaded (grxsys.h: texE7[5] at +0xE0, texE7[6] at +0xE4) */
/* GRXSYS.PAC's header (grxsys.h): a 32-bit EE address, as every pointer
   the game keeps at a PS2 offset (port/docs/win64.md) */
extern uint32_t sysGrxsysPac;

static int setting = -1; /* -1 auto, 0 PlayStation, 1 Xbox */
static int xbox_now;

typedef struct {
    int off, which;
    uint32_t hash;  /* FNV-1a of the pixels then the palette, as on the US disc */
    uint8_t *px;    /* the texture's pixels in the game's memory, when known */
    int state;      /* -2 not the expected texture, -1 not seen, 0 original, 1 Xbox */
    int recheck;
    uint8_t *orig, *xbox;
} Patch;

static Patch patches[2] = {
    {0xE0, GRX_BUTTONS_ICONS, 0x3dd09739u, NULL, -1, 0, NULL, NULL},
    {0xE4, GRX_BUTTONS_HUD, 0xb264768du, NULL, -1, 0, NULL, NULL},
};

void grx_buttons_config(const char *arg) {
    if (arg == NULL || !*arg) {
        arg = getenv("GRX_BUTTONS");
    }
    if (arg == NULL || !*arg || strcmp(arg, "auto") == 0) {
        setting = -1;
    } else if (strcmp(arg, "xbox") == 0) {
        setting = 1;
    } else if (strcmp(arg, "playstation") == 0 || strcmp(arg, "ps") == 0 || strcmp(arg, "ps2") == 0) {
        setting = 0;
    } else {
        grx_fatal("--buttons / GRX_BUTTONS: auto, xbox or playstation, not \"%s\"", arg);
    }
}

int grx_buttons_xbox(void) {
    return xbox_now;
}

static uint32_t fnv(const uint8_t *p, size_t n, uint32_t h) {
    size_t i;
    for (i = 0; i < n; i++) {
        h = (h ^ p[i]) * 0x01000193u;
    }
    return h;
}

/* Half-size PlayStation buttons for the port's own lists (the
   Configurations page's lists: grx_buttons_ps_icon), made from the
   original art into the icons texture's empty corner (x 160-255, y
   160-215, all transparent on the US disc), in both layouts: circle,
   cross, square, triangle (from the 32 x 32 discs), L1, R1, L2, R2 (from
   the 40 x 32 plaques) */
static const struct {
    int sx, sy, sw, dx, dy;
} minis[8] = {
    {85, 189, 32, 160, 160}, {85, 223, 32, 176, 160}, {119, 223, 32, 192, 160}, {119, 189, 32, 208, 160},
    {1, 189, 40, 176, 192},  {43, 189, 40, 196, 192}, {1, 223, 40, 216, 192},  {43, 223, 40, 236, 192},
};

static void add_minis(uint8_t *dst, const uint8_t *src, const uint32_t *clut) {
    static Tex t;
    int i, x, y, k, clear = 0;
    tex_init(&t, dst, clut);
    for (i = 0; i < 256 && t.rgb[i][3] != 0; i++)
        ;
    if (i < 256) {
        clear = i;
    }
    for (k = 0; k < 8; k++) {
        for (y = 0; y < 16; y++) {
            for (x = 0; x < minis[k].sw / 2; x++) {
                /* The 2 x 2 texels' colour, by their alpha, and alpha */
                float r = 0, g = 0, b = 0, a = 0;
                int j;
                for (j = 0; j < 4; j++) {
                    const uint8_t *c = t.rgb[src[(minis[k].sy + 2 * y + (j >> 1)) * TEX_W + minis[k].sx + 2 * x + (j & 1)]];
                    r += c[0] * c[3];
                    g += c[1] * c[3];
                    b += c[2] * c[3];
                    a += c[3];
                }
                PX(&t, minis[k].dx + x, minis[k].dy + y) =
                    a < 2 * 0x80 ? (uint8_t)clear : nearest(&t, r / a, g / a, b / a);
            }
        }
    }
}

/* The icons as the game's prompts draw them, in the layout's art: the discs
   from a texel right of and below their art, the plaques as they are */
static const struct {
    int u, v, w;
} icons[8] = {
    {86, 190, 32}, {86, 224, 32}, {120, 224, 32}, {120, 190, 32},
    {1, 189, 40},  {43, 189, 40}, {1, 223, 40},   {43, 223, 40},
};

int grx_buttons_icon(int i, int *u, int *v, int *w) {
    if (i < 0 || i >= 8 || patches[0].state < 0) {
        return 0;
    }
    *u = icons[i].u;
    *v = icons[i].v;
    *w = icons[i].w;
    return 1;
}

int grx_buttons_ps_icon(int i, int *u, int *v, int *w) {
    if (i < 0 || i >= 8 || patches[0].state < 0) {
        return 0;
    }
    *u = minis[i].dx;
    *v = minis[i].dy;
    *w = minis[i].sw / 2;
    return 1;
}

/* A pointer in the game's data: a 32-bit EE address on every target
   (port/docs/win64.md) */
static void *ee_load(const void *at) {
    return (void *)(uintptr_t)*(const uint32_t *)at;
}

static void patch(Patch *p, int want) {
    uint8_t *pac = (uint8_t *)(uintptr_t)sysGrxsysPac, *tex, *px;
    const uint32_t *clut;
    if (pac == NULL || (tex = ee_load(pac + p->off)) == NULL) {
        return;
    }
    /* 256 x 256, PSMT8 */
    if (((uint32_t *)tex)[0] != 256 || ((uint32_t *)tex)[1] != 256 || ((uint32_t *)tex)[4] != 0x13) {
        return;
    }
    px = ee_load(tex + 0x1C);
    clut = ee_load(tex + 0x40);
    if (px == NULL || clut == NULL) {
        return;
    }
    if (px != p->px) {
        p->px = px;
        p->state = -1;
    }
    if (p->state == -2 && ++p->recheck % 60 != 0) {
        return;
    }
    /* Not seen yet, or reloaded since (it isn't what was written) */
    if (p->state < 0 || memcmp(px, p->state ? p->xbox : p->orig, TEX_PIXELS) != 0) {
        if (fnv((const uint8_t *)clut, 1024, fnv(px, TEX_PIXELS, 0x811C9DC5u)) != p->hash) {
            if (p->state != -2) {
                grx_log("buttons: texture +0x%X isn't the one expected: its icons stay as they are", p->off);
            }
            p->state = -2;
            return;
        }
        if (p->orig == NULL) {
            p->orig = malloc(TEX_PIXELS);
            p->xbox = malloc(TEX_PIXELS);
            if (p->orig == NULL || p->xbox == NULL) {
                p->state = -2;
                return;
            }
        }
        memcpy(p->orig, px, TEX_PIXELS);
        memcpy(p->xbox, px, TEX_PIXELS);
        grx_buttons_make_xbox(p->which, p->xbox, clut);
        if (p->which == GRX_BUTTONS_ICONS) {
            add_minis(p->xbox, p->orig, clut);
            add_minis(p->orig, p->orig, clut);
        }
        /* px has neither now (the minis) */
        memcpy(px, want ? p->xbox : p->orig, TEX_PIXELS);
        p->state = want;
    }
    if (want != p->state) {
        memcpy(px, want ? p->xbox : p->orig, TEX_PIXELS);
        p->state = want;
    }
}

/* ---------------------------------------------------------------------- */
/* The title's prompt on the keyboard

   The title screen and the attract demo prompt "PRESS START BUTTON" from
   the title texture (GRXSYS.PAC +0x150: 256 x 256, PSMT4, its palette an
   alpha ramp, index 1 clear): two strips 24 rows high, "PRESS START" at
   (0,0x68) to (0xAC,0x80) and "BUTTON" at (0,0x80) to (0x68,0x98),
   SysDemoPromptPrims's quads, drawn from x 0xB0. With the keyboard in use
   the strips say "PRESS ENTER", with letters from the prompt's own (E, R
   from PRESS, N and the first T from BUTTON), or "PRESS START" for START
   on another key; the BUTTON strip is clear, and the prompt is moved
   right by half of what it lost to stay centred
   (grx_buttons_prompt_shift; port/tools/fixups/controls.py). */

#define TP_OFF 0x150
#define TP_Y 0x68               /* the strips' first row */
#define TP_ROWS 0x30
#define TP_BYTES (TP_ROWS * 128) /* 4 bits a texel */
#define TP_HASH 0x746698f4u     /* FNV-1a of those rows, as on the US disc */
#define TP_SHIFT 55             /* (0x11A - 0xAC) / 2 */

static uint8_t *tp_px;
static int tp_state = -1;       /* -2 not the expected texture, -1 not seen, else what is shown */
static int tp_recheck;
static uint8_t tp_text[3][TP_BYTES]; /* the original, PRESS ENTER, PRESS START */

static int tp_get(const uint8_t *rows, int x, int y) {
    uint8_t b = rows[y * 128 + x / 2];
    return x & 1 ? b >> 4 : b & 15;
}

static void tp_set(uint8_t *rows, int x, int y, int v) {
    uint8_t *b = &rows[y * 128 + x / 2];
    *b = (uint8_t)(x & 1 ? (*b & 0x0F) | v << 4 : (*b & 0xF0) | v);
}

/* ENTER's letters: the strip's row (0 or 0x18), the columns to copy (the
   glyph and its edge) and where they go */
static const struct {
    int y, x0, x1, to;
} enter_letters[5] = {
    {0, 35, 50, 92},     /* E of PRESS (36-49) at 93 */
    {0x18, 83, 99, 108}, /* N of BUTTON (84-98) at 109 */
    {0x18, 34, 48, 124}, /* T, BUTTON's first (35-48, cut where the next's bar starts) at 125 */
    {0, 35, 50, 140},    /* E at 141 */
    {0, 18, 35, 155},    /* R of PRESS (19-34) at 156 */
};

static void tp_make(void) {
    int x, y, k;
    uint8_t *start = tp_text[2], *enter = tp_text[1];
    memcpy(start, tp_text[0], TP_BYTES);
    for (y = 0x18; y < TP_ROWS; y++) {
        for (x = 0; x < 256; x++) {
            tp_set(start, x, y, 1);
        }
    }
    memcpy(enter, start, TP_BYTES);
    /* START out (from past PRESS's space), ENTER in, a texel at a time as
       the larger of what is there (the palette is coverage) */
    for (y = 0; y < 0x18; y++) {
        for (x = 89; x < 256; x++) {
            tp_set(enter, x, y, 1);
        }
    }
    for (k = 0; k < 5; k++) {
        for (y = 0; y < 0x18; y++) {
            for (x = enter_letters[k].x0; x <= enter_letters[k].x1; x++) {
                int to = enter_letters[k].to + x - enter_letters[k].x0;
                int v = tp_get(tp_text[0], x, enter_letters[k].y + y);
                if (v > tp_get(enter, to, y)) {
                    tp_set(enter, to, y, v);
                }
            }
        }
    }
}

static void tp_patch(int want) {
    uint8_t *pac = (uint8_t *)(uintptr_t)sysGrxsysPac, *tex, *px;
    if (pac == NULL || (tex = ee_load(pac + TP_OFF)) == NULL) {
        return;
    }
    if (((uint32_t *)tex)[0] != 256 || ((uint32_t *)tex)[1] != 256 || ((uint32_t *)tex)[4] != 0x14 ||
        (px = ee_load(tex + 0x1C)) == NULL) {
        return;
    }
    px += TP_Y * 128;
    if (px != tp_px) {
        tp_px = px;
        tp_state = -1;
    }
    if (tp_state == -2 && ++tp_recheck % 60 != 0) {
        return;
    }
    if (tp_state < 0 || memcmp(px, tp_text[tp_state], TP_BYTES) != 0) {
        if (fnv(px, TP_BYTES, 0x811C9DC5u) != TP_HASH) {
            if (tp_state != -2) {
                grx_log("buttons: the title texture isn't the one expected: its prompt stays as it is");
            }
            tp_state = -2;
            return;
        }
        memcpy(tp_text[0], px, TP_BYTES);
        tp_make();
        tp_state = 0;
    }
    if (want != tp_state) {
        memcpy(px, tp_text[want], TP_BYTES);
        tp_state = want;
    }
}

int grx_buttons_prompt_shift(void) {
    return tp_state > 0 ? TP_SHIFT : 0;
}

void grx_buttons_field(void) {
    int want = setting;
    unsigned i;
    if (want < 0) {
        /* auto: by port 1's gamepad; no gamepad keeps what there was */
        int k = grx_pad_xbox_like(0);
        want = k < 0 ? xbox_now : k;
    }
    if (want != xbox_now) {
        xbox_now = want;
        grx_log("buttons: %s layout and icons", want ? "Xbox" : "PlayStation");
    }
    for (i = 0; i < sizeof(patches) / sizeof(patches[0]); i++) {
        patch(&patches[i], xbox_now);
    }
    tp_patch(!grx_pad_keyboard() ? 0 : grx_input_get(GRX_IN_KEY, GRX_IN_START) == SDL_SCANCODE_RETURN ? 1 : 2);
}
#endif
