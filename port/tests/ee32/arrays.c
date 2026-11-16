/* Global arrays defined one after another, read past the end of the first
 * into the next as the game does (IpGaugeDraw's &ipDialRects[9 - k] is
 * ipEnemyRingRects[3 - k]): x86-64 compilers align a global array of 16
 * bytes or more to 16, which would put a gap between them (6 x 12 = 72
 * bytes, then 8 of padding). Statics too.
 * placed:
 * expect: 5 6 7 8 | 9 10
 */
#include <stdio.h>
typedef struct { short w, h; unsigned short u0, v0, u1, v1; } R;
R dial[6] = { {1}, {1}, {1}, {1}, {1}, {1} };
R ring[4] = { {5}, {6}, {7}, {8} };
static R s0[3] = { {1}, {1}, {1} };
static R s1[2] = { {9}, {10} };
/* Arrays of pointers to arrays: the element is a pointer (its type text
 * keeps a [3]), which once kept the rewrite looping */
extern R (*rows[2])[3];
R (*rows[2])[3] = { (R (*)[3])&dial[0], (R (*)[3])&dial[3] };
int main(void) {
    volatile int k = 0;
    printf("%d %d %d %d\n", dial[6 + k].w, dial[7 + k].w, dial[8 + k].w, dial[9 + k].w);
    printf("%d %d\n", s0[3 + k].w, s0[4 + k].w + 0 * s1[k].w + 0 * (*rows[k])[0].w);
    return 0;
}
