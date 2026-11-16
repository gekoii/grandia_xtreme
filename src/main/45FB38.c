#include "common.h"
#include "snd.h"
#include "protos/main.h"




extern char sndStrmLastName[];

s32 SndStrmOpen(char *name) {
    s32 info[2];
    s32 ret;
    s32 i;

    ret = SndFsFind(name, info);
    if (ret < 0) {
        return ret;
    }
    if (*name == '#') {
        name++;
        if (sndStrmSlots[3].busy != 0) {
            return -2;
        }
        i = 3;
    } else {
        for (i = 0; i < 4; i++) {
            if (sndStrmSlots[i].busy == 0 && sndIopStatus->strm[i].secNum == 0) {
                break;
            }
        }
    }
    if (i < 4) {
        sndStrmSlots[i].unk2 = 0;
        sndStrmSlots[i].lsn = info[0];
        sndStrmSlots[i].size = info[1];
        sndStrmSlots[i].busy = 1;
        for (ret = 0; name[ret] != 0; ret++) {
            sndStrmLastName[ret] = name[ret];
        }
        sndStrmLastName[ret] = 0;
        sndStrmSlots[i].pos = 0;
    } else {
        i = -2;
    }
    return i;
}
