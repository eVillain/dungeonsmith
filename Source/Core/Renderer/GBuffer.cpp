//
//  GBuffer.cpp
//  Ingenium
//
//  Created by The Drudgerist on 01/02/15.
//  Copyright (c) 2015 BitWiseOperators. All rights reserved.
//

#include "GBuffer.h"
#include <iostream>

// Helper function for the macro below to check for OpenGL errors
void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
        abort();
    }
}

#ifdef _DEBUG
#define GL_CHECK(stmt) do { \
stmt; \
CheckOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0)
#else
#define GL_CHECK(stmt) stmt
#endif

/*  ----------------------------    *
 *    OpenGL GBuffer STUFF      *
 *  ----------------------------    */
static GLuint GenerateTextureRGBAF(GLuint width, GLuint height) {
    GLuint returnTexture = -1;
    GL_CHECK( glGenTextures(1, &returnTexture) );  // Generate rgb color render texture
    GL_CHECK( glBindTexture(GL_TEXTURE_2D, returnTexture) );   // Bind current texture
    GL_CHECK( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL) );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return returnTexture;
}
static GLuint GenerateTextureDepth(GLuint width, GLuint height) {
    GLuint returnTexture = -1;
    GL_CHECK( glGenTextures(1, &returnTexture) );  // Generate rgb color render texture
    GL_CHECK( glBindTexture(GL_TEXTURE_2D, returnTexture) );   // Bind current texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    glBindTexture(GL_TEXTURE_2D, 0);
    return returnTexture;
}
static GLuint GenerateTextureNormal(GLuint width, GLuint height) {
    GLuint returnTexture = -1;
    GL_CHECK( glGenTextures(1, &returnTexture) );  // Generate rgb color render texture
    GL_CHECK( glBindTexture(GL_TEXTURE_2D, returnTexture) );   // Bind current texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    glBindTexture(GL_TEXTURE_2D, 0);
    return returnTexture;
}

void GBuffer::Initialize(GLuint width, GLuint height)
{
    _width = width;
    _height = height;
    
    /* --- Generate our frame buffer textures --- */
    _diffuse = GenerateTextureRGBAF(_width, _height);
    _specular = GenerateTextureRGBAF(_width, _height);
    _depth = GenerateTextureDepth(_width, _height);
    _normal = GenerateTextureNormal(_width, _height);
    
    // Generate main rendering frame buffer
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo); // Bind our frame buffer
    // Attach the texture render_texture to the color buffer in our frame buffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _diffuse, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, _specular, 0);
    // Attach the normal buffer to our frame buffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, _normal, 0);
    // Attach the depth buffer depth_texture to our frame buffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, _depth, 0);
    
    // Set the list of draw buffers.
    // Passing GL_DEPTH_ATTACHMENT as second buffer returns an invalid enum glerror
    GLenum DrawBuffers[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, (GLenum*)DrawBuffers);
}

void GBuffer::Terminate() {
    if (_diffuse) glDeleteTextures( 1, &_diffuse );
    if (_specular) glDeleteTextures( 1, &_specular );
    if (_depth) glDeleteTextures( 1, &_depth );
    if (_normal) glDeleteTextures( 1, &_normal );
    if (_fbo) glDeleteFramebuffers(1, &_fbo);
}

void GBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void GBuffer::UnBind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void GBuffer::Resize( GLuint width, GLuint height  ) {
    Terminate();
    Initialize(width, height);
}
