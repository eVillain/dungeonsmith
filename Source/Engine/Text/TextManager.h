//
//  TextManager.h
//  DungeonSmith
//
//  Created by The Drudgerist on 9/4/12.
//  Copyright (c) 2012 The Drudgerist. All rights reserved.
//

#ifndef NGN_TEXT_MANAGER_H
#define NGN_TEXT_MANAGER_H

#include <map>
#include <vector>
#include <string>

#include "FontAtlas.h"
#include "IText.h"

#define TEXT_FADE_TIME 0.5

class Shader;

class TextManager : public IText {
public:
    TextManager();
    ~TextManager();
    
    void Initialize();                     // Setup atlases and buffers
    void Terminate();                      // Clear atlases and delete buffers

    void Update( double delta ) {};
    void Render2D();
    void Render3D();
protected:
    void Add(TextLabel*label);
    void Remove(TextLabel*label);
    
    FontAtlas* GetAtlas( std::string filename, int size );
private:
    void Setup2DMatrix(const glm::vec3& pos, const glm::vec3& rot);
    void Setup3DMatrix(const glm::vec3& pos, const glm::vec3& rot);
    
    // FreeType library
    FT_Library ft;

    // Container for fonts
    AtlasList atlases;
    
    // This may have failed to init, we shouldn't try to render then
    bool initialized;
    
    // Counter for each frame, used for stats
    int blocksRendered;
    
    // OpenGL Shader mode vars - should move somewhere else maybe
    Shader* textShader = NULL;
    Shader* textShaderDeferred = NULL;
    
    // Container for text labels
    std::vector<TextLabel*> _labels3D;
    std::vector<TextLabel*> _labels2D;
};

#endif
