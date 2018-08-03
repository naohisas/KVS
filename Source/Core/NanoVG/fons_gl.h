//
// Copyright (c) 2009-2013 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
#ifndef GLFONTSTASH_H
#define GLFONTSTASH_H

FONScontext* glfonsCreate(int width, int height, int flags);
void glfonsDelete(FONScontext* ctx);

unsigned int glfonsRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

#endif

#ifdef GLFONTSTASH_IMPLEMENTATION

struct GLFONScontext {
    GLuint tex;
    int width, height;
};
typedef struct GLFONScontext GLFONScontext;

static int glfons__renderCreate(void* userPtr, int width, int height)
{
    GLFONScontext* gl = (GLFONScontext*)userPtr;
    // Create may be called multiple times, delete existing texture.
    if (gl->tex != 0) {
        KVS_GL_CALL( glDeleteTextures(1, &gl->tex) );
        gl->tex = 0;
    }
    KVS_GL_CALL( glGenTextures(1, &gl->tex) );
    if (!gl->tex) return 0;
    gl->width = width;
    gl->height = height;
    KVS_GL_CALL( glBindTexture(GL_TEXTURE_2D, gl->tex) );
    KVS_GL_CALL( glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, gl->width, gl->height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0) );
    KVS_GL_CALL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
    return 1;
}

static int glfons__renderResize(void* userPtr, int width, int height)
{
    // Reuse create to resize too.
    return glfons__renderCreate(userPtr, width, height);
}

static void glfons__renderUpdate(void* userPtr, int* rect, const unsigned char* data)
{
    GLFONScontext* gl = (GLFONScontext*)userPtr;
    int w = rect[2] - rect[0];
    int h = rect[3] - rect[1];

    if (gl->tex == 0) return;
    KVS_GL_CALL( glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT) );
    KVS_GL_CALL( glBindTexture(GL_TEXTURE_2D, gl->tex) );
    KVS_GL_CALL( glPixelStorei(GL_UNPACK_ALIGNMENT,1) );
    KVS_GL_CALL( glPixelStorei(GL_UNPACK_ROW_LENGTH, gl->width) );
    KVS_GL_CALL( glPixelStorei(GL_UNPACK_SKIP_PIXELS, rect[0]) );
    KVS_GL_CALL( glPixelStorei(GL_UNPACK_SKIP_ROWS, rect[1]) );
    KVS_GL_CALL( glTexSubImage2D(GL_TEXTURE_2D, 0, rect[0], rect[1], w, h, GL_ALPHA,GL_UNSIGNED_BYTE, data) );
    KVS_GL_CALL( glPopClientAttrib() );
}

static void glfons__renderDraw(void* userPtr, const float* verts, const float* tcoords, const unsigned int* colors, int nverts)
{
    GLFONScontext* gl = (GLFONScontext*)userPtr;
    if (gl->tex == 0) return;
    KVS_GL_CALL( glBindTexture(GL_TEXTURE_2D, gl->tex) );
    KVS_GL_CALL( glEnable(GL_TEXTURE_2D) );
    KVS_GL_CALL( glEnableClientState(GL_VERTEX_ARRAY) );
    KVS_GL_CALL( glEnableClientState(GL_TEXTURE_COORD_ARRAY) );
    KVS_GL_CALL( glEnableClientState(GL_COLOR_ARRAY) );

    KVS_GL_CALL( glVertexPointer(2, GL_FLOAT, sizeof(float)*2, verts) );
    KVS_GL_CALL( glTexCoordPointer(2, GL_FLOAT, sizeof(float)*2, tcoords) );
    KVS_GL_CALL( glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(unsigned int), colors) );

    KVS_GL_CALL( glDrawArrays(GL_TRIANGLES, 0, nverts) );

    KVS_GL_CALL( glDisable(GL_TEXTURE_2D) );
    KVS_GL_CALL( glDisableClientState(GL_VERTEX_ARRAY) );
    KVS_GL_CALL( glDisableClientState(GL_TEXTURE_COORD_ARRAY) );
    KVS_GL_CALL( glDisableClientState(GL_COLOR_ARRAY) );
}

static void glfons__renderDelete(void* userPtr)
{
    GLFONScontext* gl = (GLFONScontext*)userPtr;
    if (gl->tex != 0)
        KVS_GL_CALL( glDeleteTextures(1, &gl->tex) );
    gl->tex = 0;
    free(gl);
}


FONScontext* glfonsCreate(int width, int height, int flags)
{
    FONSparams params;
    GLFONScontext* gl;

    gl = (GLFONScontext*)malloc(sizeof(GLFONScontext));
    if (gl == NULL) goto error;
    memset(gl, 0, sizeof(GLFONScontext));

    memset(&params, 0, sizeof(params));
    params.width = width;
    params.height = height;
    params.flags = (unsigned char)flags;
    params.renderCreate = glfons__renderCreate;
    params.renderResize = glfons__renderResize;
    params.renderUpdate = glfons__renderUpdate;
    params.renderDraw = glfons__renderDraw; 
    params.renderDelete = glfons__renderDelete;
    params.userPtr = gl;

    return fonsCreateInternal(&params);

error:
    if (gl != NULL) free(gl);
    return NULL;
}

void glfonsDelete(FONScontext* ctx)
{
    fonsDeleteInternal(ctx);
}

unsigned int glfonsRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return (r) | (g << 8) | (b << 16) | (a << 24);
}

#endif
