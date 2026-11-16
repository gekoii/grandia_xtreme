#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void MiniConcertCarroSoundTrigger(MiniConcertActor *a, u8 n) {
    s8 k;

    if (n != 0) {
        if (n < 6) {
            k = a->unk1E;
            if ((u8)k < 5) {
                n--;
                if (miniConcertWork->unk8A94[k][n] == 0) {
                    MiniConcertCarroSePlay(k, n);
                }
                miniConcertWork->unk8A94[k][n]++;
            }
        }
    }
}
