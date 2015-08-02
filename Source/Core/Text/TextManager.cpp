//
//  TextManager.cpp
//  Bloxelizer
//
//  Created by The Drudgerist on 9/4/12.
//  Copyright (c) 2012 The Drudgerist. All rights reserved.
//

#include "TextManager.h"
#include "TextDefaults.h"
#include "HyperVisor.h"
#include "Locator.h"
#include "IRenderer.h"
#include "FileUtil.h"
#include "Shader.h"
#include "MatrixUtil.h"

#include <glm/gtc/matrix_transform.hpp>     // glm::translate, glm::rotate, glm::scale
#include <string>

#include "TextLabel.h"

TextManager::TextManager() :
initialized(false),
blocksRendered(0)
{
    textShader = NULL;
    textShaderDeferred = NULL;
}

TextManager::~TextManager()
{
    _labels2D.clear();         // Empty out all blocks
    _labels3D.clear();
    Terminate();                // Un-initialize
}

void TextManager::Initialize()
{
    if ( initialized ) {
        printf("[TextMan] can't init, already initialized\n");
        return;
    }
    
    textShader = new Shader();
    textShader->InitFromSource(text_vertex_shader_forward, text_frag_shader_forward);
    textShaderDeferred = new Shader();
    textShader->InitFromSource(text_vertex_shader_deferred, text_frag_shader_deferred);
    if ( textShader->GetProgram() == 0 ) {
        printf("[TextMan] failed to load text shader program\n");
        return;
    }
    
    // Initialize FreeType library
    if( FT_Init_FreeType(&ft) != 0 ) {
        printf("[TextMan] Could not init FreeType library\n");
        return;
    }
    initialized = true;
}

void TextManager::Terminate()
{
    if ( !initialized ) return;
    
    AtlasIter atl;
    for ( atl = atlases.begin(); atl != atlases.end(); atl++ ) {
        delete atl->second;
    }
    atlases.clear();
    FT_Done_FreeType(ft);

    delete textShader;
    textShader = NULL;
    initialized = false;
}


void TextManager::Add(TextLabel*label)
{
    if ( label->Render3D() )
    {
        _labels3D.push_back(label);
    }
    else
    {
        _labels2D.push_back(label);
    }
}

void TextManager::Remove(TextLabel*label)
{
    if ( label->Render3D() )
    {
        std::vector<TextLabel*>::iterator it = std::find(_labels3D.begin(), _labels3D.end(), label);
        if (it != _labels3D.end())
        {
            _labels3D.erase(it);
        }
    }
    else
    {
        std::vector<TextLabel*>::iterator it = std::find(_labels2D.begin(), _labels2D.end(), label);
        if (it != _labels2D.end())
        {
            _labels2D.erase(it);
        }
    }
}

/**
 Sees if the font atlas at that size was generated. If not, generates it.
 Should always return a valid font atlas if filename is legit and size isn't whack
 */
FontAtlas* TextManager::GetAtlas( std::string filename, int size ) {
    char buf[256];
#ifdef _WIN32
    sprintf_s(buf, "%s%i", filename.c_str(), size);
#else
	sprintf(buf, "%s%i", filename.c_str(), size);
#endif
	std::string fontKey = std::string(buf);
    
    FontAtlas* fAtlas = NULL;
    AtlasIter result = atlases.find(fontKey);
    if( result != atlases.end() ) {
        return result->second;
    }
    
    std::string path = FileUtil::GetPath();
    path.append("Data/Fonts/");
    path.append(filename);
    FT_Face face = NULL;
    // Load font, also known as a Face in FreeType
    if( FT_New_Face(ft, path.c_str(), 0, &face) ) {
        fprintf(stderr, "Could not open font %s\n", path.c_str() );
        return NULL;
    }
    fAtlas = new FontAtlas(face, size);
    // Clean up face
    FT_Done_Face(face);
    // Store newly created atlas
    atlases[fontKey] = fAtlas;
    return fAtlas;
}

void TextManager::Render2D() {
    if ( !initialized ) return;
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    textShader->Begin();
    for ( unsigned int i=0; i < _labels2D.size(); i++ ) {
        TextLabel& label = *_labels2D[i];
        Setup2DMatrix(label.position, label.rotation);
        textShader->setUniform4fv("color", label.color);
        label.Draw();
        blocksRendered++;
    }
    textShader->End();
    glBindVertexArray(0);
}

void TextManager::Render3D()
{
    if ( !initialized ) return;
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    textShader->Begin();
    for ( unsigned int i=0; i < _labels3D.size(); i++ ) {
        TextLabel& label = *_labels3D[i];
        Setup3DMatrix(label.position, label.rotation);
        textShader->setUniform4fv("color", label.color);
        label.Draw();
        blocksRendered++;
    }
    
    /* Unbind */
    glBindTexture( GL_TEXTURE_2D, 0 );
    textShader->End();
    glBindVertexArray(0);
}

// TODO: Check pos/rot matrix transforms in 2D and scaling in 3D
void TextManager::Setup2DMatrix(const glm::vec3& pos, const glm::vec3& rot)
{
    glm::mat4 mvp;
    glm::vec2 windowSize = Locator::getRenderer().GetWindowSize();
    MatrixUtil::GetUIMatrix(mvp, windowSize.x, windowSize.y);
    mvp = glm::translate(mvp, glm::vec3(pos.x, pos.y, pos.z) );
    mvp = glm::rotate(mvp, rot.y, glm::vec3(0.0, 1.0, 0.0) );
    mvp = glm::rotate(mvp, rot.x, glm::vec3(1.0, 0.0, 0.0) );
    mvp = glm::rotate(mvp, rot.z, glm::vec3(0.0, 0.0, 1.0) );
    // Pass MVP to shader
    textShader->setUniformM4fv("MVP", mvp);
}

void TextManager::Setup3DMatrix(const glm::vec3& pos, const glm::vec3& rot)
{
    Camera* camera = Locator::getRenderer().GetCamera();
    if ( camera == nullptr)
    {
        return;
    }
    glm::mat4 mvp;
    
    glm::vec2 windowSize = Locator::getRenderer().GetWindowSize();
    MatrixUtil::GetGameMatrix(mvp, windowSize.x, windowSize.y, *camera);
    mvp = glm::translate(mvp, glm::vec3(pos.x, pos.y, pos.z) );
    mvp = glm::rotate(mvp, rot.y, glm::vec3(0.0, 1.0, 0.0) );
    mvp = glm::rotate(mvp, rot.x, glm::vec3(1.0, 0.0, 0.0) );
    mvp = glm::rotate(mvp, rot.z, glm::vec3(0.0, 0.0, 1.0) );
    mvp = glm::scale(mvp, glm::vec3(0.01));
    // Pass MVP to shader
    textShader->setUniformM4fv("MVP", mvp);
}

