#include "common.h"
#include "main/n07.h"
#include "protos/main.h"

void FilePacModelReleaseTextures(FilePacModelPack *arg0) {
    u32 i;

    if (arg0 != NULL) {
        for (i = 0; i < arg0->texCount; i++) {
            GphTexUnregister(arg0->textures[i]);
        }
    }
}
