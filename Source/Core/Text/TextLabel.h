//
//  TextLabel.h
//  DungeonSmith
//
//  Created by The Drudgerist on 26/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__TextLabel__
#define __DungeonSmith__TextLabel__

#include <string>
#include "RenderDefines.h"
#include "Color.h"
#include "IText.h" // needed for FontName enum, clean this up later
#include "FontAtlas.h"
#include "TextConstants.h"

class TextLabel
{
    friend class TextManager;
public:
    TextLabel(std::string text,
              glm::vec3 position,
              glm::vec3 rotation = glm::vec3(0,0,0),
              Color color = COLOR_WHITE,
              Fonts::FontID font = Fonts::FONT_DEFAULT,
              int fontSize = 16,
              bool render3D = false);
    
    ~TextLabel();
    
    void SetText(const std::string& newText);
    void SetFont(const Fonts::FontID newFont);
    void SetFontSize(const int newFontSize);

    
    glm::vec3 position;
    glm::vec3 rotation;
    Color color;

    bool Render3D() { return _render3D; };
    const glm::vec2 GetSize() const;

protected:
    void Draw();
    
private:
    std::string _text;
    glm::vec2 _size;
    
    Fonts::FontID _font;
    int _fontSize;
    bool _render3D;
    
    GLuint _vao = 0;
    GLuint _vbo = 0;
    
    void BufferVertexData(FontAtlas& atlas);
    int _bufferedVerts;
    bool _dirty; // Has changed and needs to be re-buffered
};


#endif /* defined(__DungeonSmith__TextLabel__) */
