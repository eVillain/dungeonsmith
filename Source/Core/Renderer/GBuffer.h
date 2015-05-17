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

    void Resize( GLuint width, GLuint height );
    
    GLuint GetDiffuse() { return _diffuse; };
    GLuint GetSpecular() { return _specular; };
    GLuint GetDepth() { return _depth; };
    GLuint GetNormal() { return _normal; };
    
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
