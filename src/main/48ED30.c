/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct N0048ED30 {
    s32 id;
    u8 pad[0x34];
    struct N0048ED30 *next;
} N0048ED30;

typedef struct G0048ED30 {
    u8 pad[8];
    N0048ED30 *first;
    u8 pad2[8];
    struct G0048ED30 *next;
} G0048ED30;

typedef struct {
    u8 pad[0x28];
    G0048ED30 *list;
} S0048ED30;

N0048ED30 *search_svdata(s32 arg0, S0048ED30 *arg1) {
    G0048ED30 *g;
    N0048ED30 *n;

    for (g = arg1->list; g != NULL; g = g->next) {
        for (n = g->first; n != NULL; n = n->next) {
            if (n->id == arg0) {
                return n;
            }
        }
    }
    return NULL;
}
