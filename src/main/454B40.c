#include "common.h"
#include "protos/main.h"
#include "main/n08.h"

s32 McPortSync(s32 arg0) {
    s32 v[2];
    McPortState *e;
    s32 r;

    /* identical arms: without the test the copy of v[1] (move v0,v1) is lost */
    if (v) {
        e = &mcPortState[arg0];
        r = sceMcSync(1, &v[0], &v[1]);
    } else {
        e = &mcPortState[arg0];
        r = sceMcSync(1, &v[0], &v[1]);
    }
    switch (r) {
    case -1:
        break;
    case 0:
        e->busyFrames++;
        break;
    case 1:
        e->result = v[1];
        e->lastCmd = v[0];
        if (v[1] < -10) {
            e->state = 0;
        }
        if (v[0] == 1 && v[1] >= 0) {
            e->state = 1;
        }
        break;
    }
    return r;
}
