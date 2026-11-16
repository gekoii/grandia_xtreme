#include "common.h"
#include "field/n15.h"

void CamAiPresetUpdate(void *, void *);

void CamAiLadder(void *arg0, void *arg1, void *arg2) {
    CamAiPresetUpdate(arg0, &camFollow->save);
}
