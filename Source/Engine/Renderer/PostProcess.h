//
//  PostProcess.h
//  DungeonSmith
//
//  Created by The Drudgerist on 31/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__PostProcess__
#define __DungeonSmith__PostProcess__

#include "IRenderComponent.h"
#include "VertexBuffer_XYZW.h"
#include "Color.h"

class Shader;

class PostProcess : public IRenderComponent
{
public:
    
    void Initialize();
    void Terminate();
    
    void TextureFullScreen( const GLuint tex, const Color& color = COLOR_WHITE );
    void ProcessFXAA( const GLuint tex );
    void DepthTextureFullScreen( const GLuint tex, const GLfloat nearDepth, const GLfloat farDepth );
    void StencilFullScreen( const glm::vec2& windowSize );
    
private:
    Shader* _shaderFullScreenColor;
    Shader* _shaderFullScreenCopy;
    Shader* _shaderFXAA;
    Shader* _shaderDepthDebug;
    VertexBuffer_XYZW * _unitSquareBuffer;
};

#endif /* defined(__DungeonSmith__PostProcess__) */
