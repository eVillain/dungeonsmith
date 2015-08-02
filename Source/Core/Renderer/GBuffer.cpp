//
//  GBuffer.cpp
//  Ingenium
//
//  Created by The Drudgerist on 01/02/15.
//  Copyright (c) 2015 BitWiseOperators. All rights reserved.
//

#include "GBuffer.h"
#include <iostream>
#include "GLErrorUtil.h"
#include "RenderUtils.h"

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
    CHECK_GL_ERROR();

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf( "[GBuffer] Couldn't create render frame buffer, code:%i", status );
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind our new frame buffer
}

void GBuffer::Terminate()
{
    if (_diffuse) glDeleteTextures( 1, &_diffuse );
    if (_specular) glDeleteTextures( 1, &_specular );
    if (_depth) glDeleteTextures( 1, &_depth );
    if (_normal) glDeleteTextures( 1, &_normal );
    if (_fbo) glDeleteFramebuffers(1, &_fbo);
}

void GBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void GBuffer::BindDraw()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
}

void GBuffer::BindRead()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);;
}

void GBuffer::UnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::Clear()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GBuffer::Resize( GLuint width, GLuint height  )
{
    Terminate();
    Initialize(width, height);
}
