/* The scene skip prompt's picture (port/src/skip_overlay.c), composed with
   the presenter's colours over a grey checkerboard, as PPM, to look at.

   Build (from the repository root):
     i686-w64-mingw32-clang -std=gnu11 -Wall -O1 -Iport/include \
         port/tests/skip_art.c port/src/skip_overlay.c -o skip_art.exe
   Run: skip_art.exe OUT.ppm [TEXT_PX] (all the prompts, one under another,
   the bar at 40%) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grx_skip.h"

#define W 1200
#define H 640

static float img[H][W][3];

/* skip.c's shader, on the CPU */
static void put(const unsigned char *p, int pw, int ph, int ox, int oy, float fill_x) {
    int x, y, c;
    for (y = 0; y < ph && oy + y < H; y++) {
        for (x = 0; x < pw && ox + x < W; x++) {
            const unsigned char *t = p + ((size_t)y * (size_t)pw + (size_t)x) * 4;
            float tr = t[0] / 255.0f, tg = t[1] / 255.0f, tb = t[2] / 255.0f;
            float col[3] = {0.035f * 0.8f * tg, 0.04f * 0.8f * tg, 0.055f * 0.8f * tg}, a = 0.8f * tg;
            float fill = (float)x + 0.5f < fill_x ? 1.0f : 0.0f;
            float ba = tb * (fill > 0 ? 1.0f : 0.3f);
            float bc[3] = {fill > 0 ? 1.0f : 1.0f, fill > 0 ? 0.80f : 1.0f, fill > 0 ? 0.36f : 1.0f};
            for (c = 0; c < 3; c++) {
                col[c] = bc[c] * ba + col[c] * (1 - ba);
            }
            a = ba + a * (1 - ba);
            for (c = 0; c < 3; c++) {
                col[c] = 0.96f * tr + col[c] * (1 - 0.96f * tr);
            }
            a = 0.96f * tr + a * (1 - 0.96f * tr);
            for (c = 0; c < 3; c++) {
                float *d = &img[oy + y][ox + x][c];
                *d = col[c] + *d * (1 - a);
            }
        }
    }
}

int main(int argc, char **argv) {
    int px = argc > 2 ? atoi(argv[2]) : 24, x, y, k, oy = 20;
    FILE *f;
    if (argc < 2) {
        fprintf(stderr, "usage: skip_art OUT.ppm [TEXT_PX]\n");
        return 2;
    }
    for (y = 0; y < H; y++) {
        for (x = 0; x < W; x++) {
            float v = ((x / 32 + y / 32) & 1) ? 0.55f : 0.35f;
            img[y][x][0] = v;
            img[y][x][1] = v * 0.9f;
            img[y][x][2] = v * 0.8f;
        }
    }
    for (k = 0; k < 4; k++) {
        int w, h, b0, b1;
        unsigned char *p = grx_skip_art(px, k == 3, k < 3 ? k : 0, &w, &h, &b0, &b1);
        if (p == NULL) {
            return 1;
        }
        put(p, w, h, 20, oy, b0 + 0.4f * (float)(b1 - b0));
        printf("kind %d button %d: %dx%d, bar %d-%d\n", k == 3, k, w, h, b0, b1);
        oy += h + 20;
        free(p);
    }
    f = fopen(argv[1], "wb");
    if (f == NULL) {
        return 1;
    }
    fprintf(f, "P6\n%d %d\n255\n", W, H);
    for (y = 0; y < H; y++) {
        for (x = 0; x < W; x++) {
            for (k = 0; k < 3; k++) {
                float v = img[y][x][k];
                fputc((int)((v < 0 ? 0 : v > 1 ? 1 : v) * 255 + 0.5f), f);
            }
        }
    }
    fclose(f);
    return 0;
}
