/* Fields: reads, writes, +=, ++/--, arrays of pointers, calls through a function-pointer field, static initialisers, macros.
 * placed: 
 * expect: 311423242 g=5 size=24 abc
 */
#include <stdio.h>
#include "fields.h"
#define ARG(x) (x)
#define NEXTV(p) ((p)->next->v)
#define NEXTP(p) ((p)->next)
#define NIL ((void *)0)
#define RELOC(x, b) { unsigned v_ = (unsigned)(x); v_ += (unsigned)(b); (x) = (__typeof__(x))v_; }
#define CUR (tab[1].next)
int g;
void f1(int x) { g += x; }
Node tab[3] = { { &tab[1], 1, { "a", 0, "c" }, f1 }, { &tab[2], 2 }, { 0, 3 } };
static const Pair cp = { &tab[0], &tab[2], 7 };
int arr[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
typedef struct { int *p; } IP;
int use(Node *p) {
    int r = 0;
    Node *q = p->next->next;
    r += q->v;                       /* 3 */
    p->next = q;                     /* tab0 -> tab2 */
    r += p->next->v * 10;            /* 30 */
    { IP ip = { arr }; ip.p += 2; r += *ip.p * 100; ip.p++; r += *ip.p * 1000; --ip.p; r += *(ip.p++) * 10000; r += ip.p[1] * 100000; }
    p->names[1] = "b";
    r += (p->names[1][0] == 'b') * 1000000;
    p->fn(5);
    r += (ARG(p->next) == &tab[2]) * 10000000;
    r += cp.a->v + cp.b->v * 2;      /* 1 + 6 */
    r += NEXTV(&tab[1]) * 100000000; /* 3 */
    if (NEXTP(&tab[1]) == &tab[2]) r += 0; else r -= 5;
    tab[2].next = NIL;
    tab[1].next = tab[0].next;
    r += CUR->v * 1000000000 / 1000000000;
    CUR = &tab[1];
    { Pair pr = { 0, 0, 0 }; pr.a = (Node *)0; RELOC(pr.a, &tab[2]); r += pr.a->v; }
    return r;
}
int main(void) {
    int r = use(&tab[0]);
    printf("%d g=%d size=%d %s%s%s\n", r, g, (int)sizeof(Node), tab[0].names[0], tab[0].names[1], tab[0].names[2]);
    return 0;
}
