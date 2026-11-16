#!/bin/sh
# Run a command in the build container with the repo mounted, e.g.
#   tools/docker.sh make -j8
#
# Commands run in one long-lived container (grandia-worker) via `docker exec`,
# which saves the startup cost of a fresh container on every call. The worker
# is started on first use and can be stopped with `docker rm -f grandia-worker`.
cd "$(dirname "$0")/.."
PATH="$PATH:/Applications/Docker.app/Contents/Resources/bin"
WORKER=grandia-worker

docker image inspect grandia-build >/dev/null 2>&1 || docker build --platform linux/amd64 -t grandia-build .

if [ "$(docker inspect -f '{{.State.Running}}' "$WORKER" 2>/dev/null)" != "true" ]; then
    docker rm -f "$WORKER" >/dev/null 2>&1
    # Two callers may race to start it; whoever loses just uses the winner's.
    docker run -d --name "$WORKER" --platform linux/amd64 -v "$PWD":/grandia -w /grandia \
        grandia-build sleep infinity >/dev/null 2>&1
    i=0
    until [ "$(docker inspect -f '{{.State.Running}}' "$WORKER" 2>/dev/null)" = "true" ] || [ $i -ge 50 ]; do
        sleep 0.1; i=$((i + 1))
    done
fi

exec docker exec -w /grandia "$WORKER" "$@"
