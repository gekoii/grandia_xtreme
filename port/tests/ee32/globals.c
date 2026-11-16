/* Pointer globals at PS2 offsets, a local that shadows one, an initialised one.
 * placed: gData gEnt gp
 * expect: 20 2 20 4 | a
 */
#include <stdio.h>
typedef struct { char name[8]; int size; void *data; } Ent;
Ent tab[2] = { { "a", 1, 0 }, { "b", 2, 0 } };
extern void *gData;
void *gData;
extern Ent *gEnt;
Ent *gEnt;
int arr[4] = { 10, 20, 30, 40 };
static int *gp = arr;
int main(void) {
    int *q;
    gData = arr;
    gEnt = &tab[1];
    q = gData;
    q += 1;
    gp++;
    printf("%d %d %d %d\n", *q, gEnt->size, *gp, (int)sizeof(gEnt));
    { void *gData = &tab[0]; printf("%s\n", ((Ent *)gData)->name); }
    return 0;
}
