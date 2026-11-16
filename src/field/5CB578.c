#include "common.h"

/* Unprototyped: the stub hands its own a0/a1 (camera, look-at) on to
   CamAiFollowUpdate (0x005CC830) without touching them. */
void CamAiFollowUpdate();

void CamAiFree(void *obj, void *vec) {

}

void CamAiFollowStub(void) {
    CamAiFollowUpdate();
}
