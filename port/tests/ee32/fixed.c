/* Pointers kept in 32-bit slots that aren't pointer fields: at a fixed
 * address (*(T **)ADDR, also in a macro), in an s32 field
 * (*(T **)&s->s32field), in a table of s32 ((T **)tab)[i]. Each is 4 bytes,
 * so the next word keeps its value (0x1234, 3, 5; the original's 8-byte
 * stores clear them).
 * placed: x
 * expect: 7 1234 7 | 7 | 7 3 | 7 5
 */
#include <stdio.h>
typedef struct { int v; } W;
W wa = { 7 };
unsigned int spr[8];
typedef struct { int a; int s32f; int b; } S3;
S3 s3 = { 1, 0, 3 };
int tab[3] = { 0, 0, 5 };
#define MGR (*(W **)ADDR)
#define ADDR ((unsigned long long)&spr[3])
int main(void) {
    W **pp;
    spr[4] = 0x1234;
    *(W *volatile *)ADDR = &wa;
    printf("%d %x %d\n", (*(W **)ADDR)->v, spr[4], MGR->v);
    pp = (W **)ADDR;
    printf("%d\n", (*pp)->v);
    *(W **)&s3.s32f = &wa;
    printf("%d %d\n", (*(W **)&s3.s32f)->v, s3.b);
    ((W **)tab)[1] = &wa;
    printf("%d %d\n", ((W **)tab)[1]->v, tab[2]);
    return 0;
}
