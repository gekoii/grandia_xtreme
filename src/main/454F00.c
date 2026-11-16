#include "common.h"
#include "protos/main.h"
#include "main/n08.h"


void McExecNext(void) {
    McReq *cmd;
    McPortState *ctx;

    cmd = McQueuePop();
    if (cmd == NULL) {
        return;
    }
    cmd->status = 1;
    ctx = McGetCurPort();
    mcCurReq = cmd;
    cmd->port = ctx->port;
    cmd->slot = ctx->slot;
    mcSyncResult = 0;
    switch ((s16)(cmd->cmd - 1)) {
    case 0:
        sceMcGetInfo(ctx->port, ctx->slot, cmd->arg[0], cmd->arg[1], cmd->arg[2]);
        break;
    case 1:
        cmd->fd = sceMcOpen(ctx->port, ctx->slot, cmd->arg[0], cmd->arg[1]);
        break;
    case 2:
        sceMcClose(McReqLookup(cmd->arg[0])->fd);
        break;
    case 3:
        sceMcSeek(McReqLookup(cmd->arg[0])->fd, cmd->arg[1], cmd->arg[2]);
        break;
    case 4:
        sceMcRead(McReqLookup(cmd->arg[0])->fd, cmd->arg[1], cmd->arg[2]);
        break;
    case 5:
        sceMcWrite(McReqLookup(cmd->arg[0])->fd, cmd->arg[1], cmd->arg[2]);
        break;
    case 9:
        sceMcFlush(McReqLookup(cmd->arg[0])->fd);
        break;
    case 10:
        sceMcMkdir(ctx->port, ctx->slot, cmd->arg[0]);
        break;
    case 11:
        sceMcChdir(ctx->port, ctx->slot, cmd->arg[0], cmd->arg[1]);
        break;
    case 12:
        sceMcGetDir(ctx->port, ctx->slot, cmd->arg[0], cmd->arg[1], cmd->arg[2], cmd->arg[3]);
        break;
    case 13:
        sceMcSetFileInfo(ctx->port, ctx->slot, cmd->arg[0], cmd->arg[1], cmd->arg[2]);
        break;
    case 14:
        sceMcDelete(ctx->port, ctx->slot, cmd->arg[0]);
        break;
    case 15:
        sceMcFormat(ctx->port, ctx->slot);
        break;
    case 16:
        sceMcUnformat(ctx->port, ctx->slot);
        break;
    case 17:
        sceMcGetEntSpace(ctx->port, ctx->slot, cmd->arg[0]);
        break;
    case 18:
        sceMcRename(ctx->port, ctx->slot, cmd->arg[0], cmd->arg[1]);
        break;
    case 19:
        sceMcChangeThreadPriority(cmd->arg[0]);
        break;
    }
}
