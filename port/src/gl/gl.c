/* The OpenGL loader (grx_gl.h): the functions GRX_GL_FUNCS lists, through
   SDL_GL_GetProcAddress, and a shader-program helper. */
#include <stdlib.h>
#include <SDL3/SDL.h>
#include "grx_gl.h"
#include "grx_platform.h"

#define GRX_GL_DEFINE(ret, name, params) grx_PFNgl##name gl##name;
GRX_GL_FUNCS(GRX_GL_DEFINE)
#undef GRX_GL_DEFINE
grx_PFNglTextureBarrier glTextureBarrier;
grx_PFNglClipControl glClipControl;

int grx_gl_has(const char *ext) {
    GLint n = 0, i;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);
    for (i = 0; i < n; i++) {
        const char *e = (const char *)glGetStringi(GL_EXTENSIONS, (GLuint)i);
        if (e != NULL && SDL_strcmp(e, ext) == 0) {
            return 1;
        }
    }
    return 0;
}

int grx_gl_load(void) {
#define GRX_GL_LOAD(ret, name, params)                                                                            \
    gl##name = (grx_PFNgl##name)SDL_GL_GetProcAddress("gl" #name);                                                \
    if (gl##name == NULL) {                                                                                       \
        grx_log("gl: no gl" #name " (%s)", SDL_GetError());                                                       \
        return -1;                                                                                                \
    }
    GRX_GL_FUNCS(GRX_GL_LOAD)
#undef GRX_GL_LOAD
    glTextureBarrier = NULL;
    if (grx_gl_has("GL_ARB_texture_barrier")) {
        glTextureBarrier = (grx_PFNglTextureBarrier)SDL_GL_GetProcAddress("glTextureBarrier");
    }
    if (glTextureBarrier == NULL && grx_gl_has("GL_NV_texture_barrier")) {
        glTextureBarrier = (grx_PFNglTextureBarrier)SDL_GL_GetProcAddress("glTextureBarrierNV");
    }
    glClipControl = NULL;
    if (grx_gl_has("GL_ARB_clip_control")) {
        glClipControl = (grx_PFNglClipControl)SDL_GL_GetProcAddress("glClipControl");
    }
    return 0;
}

static GLuint shader(GLenum type, const char *src, const char *what) {
    GLuint s = glCreateShader(type);
    GLint ok = 0, len = 0;

    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char *log;
        glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);
        log = malloc(len > 0 ? (size_t)len : 1);
        if (log != NULL) {
            log[0] = 0;
            glGetShaderInfoLog(s, len, NULL, log);
        }
        grx_log("gl: %s's %s shader: %s", what, type == GL_VERTEX_SHADER ? "vertex" : "fragment",
                log != NULL ? log : "?");
        free(log);
        glDeleteShader(s);
        return 0;
    }
    return s;
}

GLuint grx_gl_program(const char *vertex, const char *fragment, const char *what) {
    GLuint vs = shader(GL_VERTEX_SHADER, vertex, what), fs, p;
    GLint ok = 0, len = 0;

    if (vs == 0) {
        return 0;
    }
    fs = shader(GL_FRAGMENT_SHADER, fragment, what);
    if (fs == 0) {
        glDeleteShader(vs);
        return 0;
    }
    p = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char *log;
        glGetProgramiv(p, GL_INFO_LOG_LENGTH, &len);
        log = malloc(len > 0 ? (size_t)len : 1);
        if (log != NULL) {
            log[0] = 0;
            glGetProgramInfoLog(p, len, NULL, log);
        }
        grx_log("gl: %s's program: %s", what, log != NULL ? log : "?");
        free(log);
        glDeleteProgram(p);
        return 0;
    }
    return p;
}
