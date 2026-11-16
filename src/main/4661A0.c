#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

typedef struct {
    s64 unk0;
    MovPts p;
} Obj;


s32 mpegTS(s32 arg0, Obj *arg1) {
    MovPts t[2];
    viBufGetTs(&movVideoDecViBuf, t);
    arg1->p = t[0];
    return 1;
}
