//
//  PostProcess.cpp
//  DungeonSmith
//
//  Created by eVillain on 31/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "PostProcess.h"
#include "ShaderFactory.h"
#include "Primitives2D.h"
#include "StencilUtils.h"

static const glm::vec4 squareVerts[4] = {
    glm::vec4(-1.0,-1.0, 0.0, 1.0),  // BL
    glm::vec4( 1.0,-1.0, 0.0, 1.0),  // BR
    glm::vec4( 1.0, 1.0, 0.0, 1.0),  // TR
    glm::vec4(-1.0, 1.0, 0.0, 1.0),  // TL
};

void PostProcess::Initialize()
{
    _shaderFullScreenColor = ShaderFactory::LoadFromFile("forward_fs_uColor.fsh", "forward_fs.vsh");
    _shaderFullScreenCopy = ShaderFactory::LoadFromFile("forward_fs_tex_uColor.fsh", "forward_fs_tex.vsh");
    _shaderFXAA = ShaderFactory::LoadFromFile("forward_FXAA.fsh", "forward_FXAA.vsh");
    _shaderDepthDebug = ShaderFactory::LoadFromFile("forward_depth_debug.fsh", "forward_fs_tex.vsh");
    
    _unitSquareBuffer = new VertexBuffer_XYZW(4);
    _unitSquareBuffer->Bind();
    _unitSquareBuffer->Buffer(*squareVerts, 4);
    _unitSquareBuffer->Upload();
    _unitSquareBuffer->Unbind();
}

void PostProcess::Terminate()
{
    ShaderFactory::ClearShader(_shaderFullScreenCopy);
    ShaderFactory::ClearShader(_shaderFXAA);
    delete _unitSquareBuffer;
    _unitSquareBuffer = nullptr;
}

void PostProcess::TextureFullScreen( const GLuint tex, const Color& color ) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    _unitSquareBuffer->Bind();
    _shaderFullScreenCopy->Begin();
    
    _shaderFullScreenCopy->setUniform4fv("uColor", color);
    _shaderFullScreenCopy->setUniform1iv("textureMap", 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    _shaderFullScreenCopy->End();
    _unitSquareBuffer->Unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void PostProcess::ProcessFXAA( const GLuint tex ) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    _unitSquareBuffer->Bind();
    _shaderFXAA->Begin();
    
    _shaderFXAA->setUniform1iv("textureMap", 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    _unitSquareBuffer->Unbind();
    _shaderFXAA->End();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void PostProcess::DepthTextureFullScreen( const GLuint tex, const GLfloat nearDepth, const GLfloat farDepth ) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    _unitSquareBuffer->Bind();
    _shaderDepthDebug->Begin();
    
    _shaderDepthDebug->setUniform1iv("textureMap", 0);
    _shaderDepthDebug->setUniform1fv("nearDepth", nearDepth);
    _shaderDepthDebug->setUniform1fv("farDepth", farDepth);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    _shaderDepthDebug->End();
    _unitSquareBuffer->Unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void PostProcess::StencilFullScreen( const glm::vec2& windowSize ) {
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    
    _unitSquareBuffer->Bind();
    _shaderFullScreenColor->Begin();
    
    _shaderFullScreenColor->setUniform4fv("uColor", COLOR_GREY);
    glStencilFunc(GL_EQUAL, Stencil::Layer_Clear, 0xFF);        // Only draw current layer
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    _shaderFullScreenColor->setUniform4fv("uColor", COLOR_BLUE);
    glStencilFunc(GL_EQUAL, Stencil::Layer_Sky, 0xFF);          // Only draw current layer
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    _shaderFullScreenColor->setUniform4fv("uColor", COLOR_BROWN);
    glStencilFunc(GL_EQUAL, Stencil::Layer_Solid, 0xFF);        // Only draw current layer
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    _shaderFullScreenColor->setUniform4fv("uColor", COLOR_YELLOW);
    glStencilFunc(GL_EQUAL, Stencil::Layer_Light, 0xFF);        // Only draw current layer
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    _shaderFullScreenColor->setUniform4fv("uColor", COLOR_PURPLE);
    glStencilFunc(GL_EQUAL, Stencil::Layer_Transparent, 0xFF);        // Only draw current layer
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    _shaderFullScreenColor->End();
    _unitSquareBuffer->Unbind();

    glDisable(GL_STENCIL_TEST);
}
