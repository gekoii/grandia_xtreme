/* A T ** global stepping through an array of them; a table's T ** fields; MemSet over the array.
 * placed: list listPtr ga gb
 * expect: 1 2 2 2 1 | 2
 */
#include <stdio.h>
typedef struct { int v; } W;
W wa = { 1 }, wb = { 2 };
W *list[4];
W **listPtr;
W *ga, *gb;
typedef struct { int k; W **ref; } Def;
typedef struct { int n; W **gens; } Sys;
Sys sys = { 2, 0 };
Def defs[2] = { { 1, &ga }, { 2, &gb } };
void MemSet(void *p, int c, int n) { char *q = p; while (n--) *q++ = c; }
int main(void) {
    MemSet(list, 0, sizeof(list));
    listPtr = list;
    *listPtr++ = &wa;
    *listPtr++ = &wb;
    ga = &wb; gb = &wa;
    printf("%d %d %d %d %d\n", list[0]->v, list[1]->v, (int)(listPtr - list), (*defs[0].ref)->v, defs[1].ref[0]->v);
    sys.gens = list;
    { W **p = sys.gens; p++; printf("%d\n", (*p)->v); }
    return 0;
}
