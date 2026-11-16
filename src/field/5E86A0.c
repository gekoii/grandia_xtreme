#include "common.h"
#include "protos/field.h"
#include "gph.h"


void ChrModelShowExtraParts(void *arg0, u8 arg1) {
    GphPart *n;

    if (arg1 == 1) {
        n = ChrModelFindPart(arg0, 0x366);
        if (n != 0) {
            if (n->flags & 8) {
                n->flags ^= 8;
            }
            n->color[3] = 0;
        }
        n = ChrModelFindPart(arg0, 0x302);
        if (n != 0) {
            if (n->flags & 8) {
                n->flags ^= 8;
            }
            n->color[3] = 0;
        }
        n = ChrModelFindPart(arg0, 0x30C);
        if (n != 0) {
            if (n->flags & 8) {
                n->flags ^= 8;
            }
            n->color[3] = 0;
        }
    } else {
        n = ChrModelFindPart(arg0, 0x366);
        if (n != 0) {
            n->flags |= 8;
            n->color[3] = 0;
        }
        n = ChrModelFindPart(arg0, 0x302);
        if (n != 0) {
            n->flags |= 8;
            n->color[3] = 0;
        }
        n = ChrModelFindPart(arg0, 0x30C);
        if (n != 0) {
            n->flags |= 8;
            n->color[3] = 0;
        }
    }
}
