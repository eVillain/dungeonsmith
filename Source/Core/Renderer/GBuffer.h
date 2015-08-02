//
//  GBuffer.h
//  Ingenium
//
//  Created by The Drudgerist on 01/02/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __GBuffer__
#define __GBuffer__

#include "RenderDefines.h"

class GBuffer {
public:
    void Initialize(GLuint width, GLuint height);
    void Terminate();
    
    void Bind();
    void Clear();
    void UnBind();

    void BindDraw();
    void BindRead();
    
    void Resize( GLuint width, GLuint height );
    
    const GLuint GetFBO() const { return _fbo; };
    
    const GLuint GetDiffuse() const { return _diffuse; };
    const GLuint GetSpecular() const { return _specular; };
    const GLuint GetDepth() const { return _depth; };
    const GLuint GetNormal() const { return _normal; };
    
private:
    GLuint _width;
    GLuint _height;
    GLuint _fbo;
    GLuint _diffuse;
    GLuint _specular;
    GLuint _depth;
    GLuint _normal;    
};
#endif /* defined(__Ingenium__FrameBuffer__) */
