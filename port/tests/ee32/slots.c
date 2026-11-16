/* A local T ** walking an array of 32-bit pointers; a function-pointer global.
 * placed: g2 hook
 * expect: hook | 1 2 2
 */
#include <stdio.h>
typedef struct { int v; } W;
W wa = { 1 }, wb = { 2 };
W *g2[2];
void (*hook)(void);
void f(void) { puts("hook"); }
int main(void) {
    W **p = &g2[0];
    void **q;
    *p++ = &wa;
    *p = &wb;
    q = (void **)g2;
    hook = f;
    hook();
    printf("%d %d %d\n", g2[0]->v, g2[1]->v, ((W *)q[1])->v);
    return 0;
}
