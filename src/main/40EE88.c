#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n01.h"


void SprFlush(void) {
    if (sprQuadCount > 0) {
        sprQuadList[sprQuadCount].type = 0;
        GphBatchDrawQuads(sprQuadList);
        sprQuadCount = 0;
    }
    if (sprRectCount > 0) {
        sprRectList[sprRectCount].type = 0;
        GphBatchDrawSprites(sprRectList);
        sprRectCount = 0;
    }
    if (sprQuad3DCount > 0) {
        sprQuad3DList[sprQuad3DCount].type = 0;
        GphBatchDrawQuads3D(sprQuad3DList);
        sprQuad3DCount = 0;
    }
}
