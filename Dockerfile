# Build environment: MIPS binutils, the 32-bit x86 ee-gcc compilers and m2c.
# The compilers are i386 Linux binaries, so this image is linux/amd64
# (Docker Desktop emulates it on Apple Silicon).
FROM --platform=linux/amd64 ubuntu:24.04

RUN dpkg --add-architecture i386 && apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        binutils-mips-linux-gnu cpp libc6:i386 make python3 python3-venv python3-pip ca-certificates && \
    rm -rf /var/lib/apt/lists/*

# m2c (MIPS assembly -> first-draft C), pinned to a known commit
ARG M2C_COMMIT=708d2d2cb2698f091a92492b328f73b24209f72d
RUN python3 -m venv /opt/venv && \
    /opt/venv/bin/pip install --no-cache-dir \
        "https://github.com/matt-kempster/m2c/archive/${M2C_COMMIT}.tar.gz"
ENV PATH="/opt/venv/bin:${PATH}"

# decomp-permuter (randomly rewrites C to search for a matching variant), pinned
ARG PERMUTER_COMMIT=8556c81d80d1c1af98a858c8f4dc951357f29139
RUN /opt/venv/bin/pip install --no-cache-dir pynacl toml Levenshtein pycparser && \
    mkdir -p /opt/permuter && \
    python3 -c "import urllib.request; urllib.request.urlretrieve('https://github.com/simonlindholm/decomp-permuter/archive/${PERMUTER_COMMIT}.tar.gz', '/tmp/permuter.tar.gz')" && \
    tar -xzf /tmp/permuter.tar.gz -C /opt/permuter --strip-components=1 && rm /tmp/permuter.tar.gz

WORKDIR /grandia
