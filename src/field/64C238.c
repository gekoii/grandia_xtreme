#include "common.h"
#include "protos/field.h"

extern u16 miniConcertCarroSeBase[];

s32 MiniConcertCarroSePlay(s8 arg0, u8 arg1) {
    if ((u8)arg0 >= 5 || arg1 >= 5) {
        return 0;
    }
    SndSePlay3D((u16)(miniConcertCarroSeBase[arg0] + arg1), 0, 0, 0);
    return 1;
}
