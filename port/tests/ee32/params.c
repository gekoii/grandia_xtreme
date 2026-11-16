/* SLOT_PARAMS: MenuTextListMeasure/Draw take a struct of TxtObj pointers as a TxtObj **.
 * placed: x
 * expect: 132
 */
#include <stdio.h>
typedef struct TxtObj { int width; } TxtObj;
typedef struct { TxtObj *a, *b, *c, *end; } Label;
TxtObj t0 = { 0 }, t1 = { 5 }, t2 = { 7 };
Label lab;
void MenuTextListMeasure(TxtObj **arg0);
void MenuTextListMeasure(TxtObj **arg0) {
    TxtObj **pp;
    TxtObj *t = arg0[0];
    t->width = 0;
    for (pp = &arg0[1]; *pp != NULL; pp++) t->width += (*pp)->width;
}
void MenuTextListDraw(TxtObj **arg0) {
    TxtObj **pp = arg0 + 1;
    int n = 0;
    while (*pp != NULL) { n += (*pp)->width * 10; pp++; }
    arg0[0]->width += n;
}
int main(void) {
    lab.a = &t0; lab.b = &t1; lab.c = &t2; lab.end = 0;
    MenuTextListMeasure((TxtObj **)&lab);
    MenuTextListDraw((TxtObj **)&lab);
    printf("%d\n", t0.width);
    return 0;
}
