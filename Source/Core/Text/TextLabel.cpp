//
//  TextLabel.cpp
//  DungeonSmith
//
//  Created by eVillain on 26/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "TextLabel.h"
#include "Locator.h"
#include "IText.h"

TextLabel::TextLabel(std::string text,
                     glm::vec3 position,
                     glm::vec3 rotation,
                     Color color,
                     Fonts::FontID font,
                     int fontSize,
                     bool render3D) :
_text(text),
position(position),
rotation(rotation),
color(color),
_font(font),
_fontSize(fontSize),
_render3D(render3D)
{
    Locator::getText().Add(this);
    
    // Create vertex array object
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    // Create the vertex buffer object
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    glBindVertexArray(0);
    
    _dirty = true;
}

TextLabel::~TextLabel()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
    Locator::getText().Remove(this);
}

void TextLabel::SetText(const std::string& newText)
{
    _text = newText;
    _dirty = true;
}

void TextLabel::SetFont(const Fonts::FontID newFont)
{
    _font = newFont;
    _dirty = true;
}

void TextLabel::SetFontSize(const int newFontSize)
{
    _fontSize = newFontSize;
    _dirty = true;
}

glm::vec2 TextLabel::GetSize()
{
    // Re-calculate size if dirty, otherwise we already know it
    if ( _dirty )
    {
        float height = 0;
        float width = 0;
        float widthMax = 0;
        std::string fontFile = Fonts::GetFileName( _font );
        FontAtlas* a = Locator::getText().GetAtlas(fontFile, _fontSize);
        GlyphInfo* g = a->GetGlyphInfo();
        const uint8_t *p;
        
        // TODO: check for longest width line
        for (p = (const uint8_t *)_text.c_str(); *p; p++) {
            /* Newline character - move cursor to beginning(left) and down one row */
            if ( strncmp((const char*)p, "\n", 1) == 0)
            {
                height += _fontSize;
                if ( width > widthMax ) { widthMax = width; }
                width = 0;
                continue;
            }
            width += g[*p].ax;
            float h = g[*p].bh;
            if ( height < h ) height = h;
        }
        if ( width > widthMax ) { widthMax = width; }
        _size = glm::vec2(widthMax, height);
    }

    return _size;
}

void TextLabel::Draw()
{
    // Bind our vertex array and vertex buffer
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    
    /* Use the texture containing the atlas */
    std::string fontName = Fonts::GetFileName(_font);
    FontAtlas* atlas = Locator::getText().GetAtlas(fontName, _fontSize);
    glBindTexture( GL_TEXTURE_2D, atlas->GetTextureID() );
    
    /* Refresh vertices if text or font has been modified */
    if ( _dirty )
    { BufferVertexData(*atlas); }
    /* Draw verts */
    glDrawArrays(GL_TRIANGLES, 0, _bufferedVerts);
}

void TextLabel::BufferVertexData(FontAtlas& atlas)
{
    _bufferedVerts = 0;

    const uint8_t *p;
	unsigned long length = _text.length();
    /* Temporary buffers for vertices */
	glm::vec4* coords = new glm::vec4[6 * length];
    glm::vec2* tCoords = new glm::vec2[6 * length];
    GlyphInfo* g = atlas.GetGlyphInfo();
    float x = 0;
    float y = 0;
    float z = 0;
    float height = 0;
    float width = 0;
    float widthMax = 0;
    
    /* Loop through all characters */
    for (p = (const uint8_t *)_text.c_str(); *p; p++)
    {
        /* Newline character */
        if ( strncmp((const char*)p, "\n", 1) == 0)
        {
            /* Move cursor to beginning(left) and down one row */
            x = 0;
            y -= _fontSize;
            
            /* Size calculation */
            height += _fontSize;
            if ( width > widthMax ) { widthMax = width; }
            width = 0;

            continue;
        }
        
        /* Calculate the vertex and texture coordinates */
        float x2 = x + g[*p].bl;
        float y2 = -y - g[*p].bt;
        float w = g[*p].bw;
        float h = g[*p].bh;
        
        /* Advance the cursor to the start of the next character */
        x += g[*p].ax;
        y += g[*p].ay;
        
        /* Skip glyphs that have no pixels */
        if (!w || !h)
        { continue; }
        
        int aw = atlas.GetWidth();
        int ah = atlas.GetHeight();
        /* Pack coords into temporary buffers */
        tCoords[_bufferedVerts] = glm::vec2(g[*p].tx + g[*p].bw / aw, g[*p].ty);
		coords[_bufferedVerts++] = glm::vec4( x2+w, -y2, z, 1.0f );
		tCoords[_bufferedVerts] = glm::vec2(g[*p].tx, g[*p].ty);
		coords[_bufferedVerts++] = glm::vec4( x2, -y2, z, 1.0f );
        tCoords[_bufferedVerts] = glm::vec2(g[*p].tx, g[*p].ty + g[*p].bh / ah);
		coords[_bufferedVerts++] = glm::vec4( x2, -y2-h, z, 1.0f );
        tCoords[_bufferedVerts] = glm::vec2(g[*p].tx + g[*p].bw / aw, g[*p].ty);
		coords[_bufferedVerts++] = glm::vec4( x2+w, -y2, z, 1.0f );
        tCoords[_bufferedVerts] = glm::vec2(g[*p].tx, g[*p].ty + g[*p].bh / ah);
		coords[_bufferedVerts++] = glm::vec4( x2, -y2-h, z, 1.0f );
        tCoords[_bufferedVerts] = glm::vec2(g[*p].tx + g[*p].bw / aw, g[*p].ty + g[*p].bh / ah);
		coords[_bufferedVerts++] = glm::vec4( x2+w, -y2-h, z, 1.0f );
        
        /* Size calculation */
        width += x;
        if ( height < h ) height = h;
    }
    if ( width > widthMax ) { widthMax = width; }
    _size = glm::vec2(widthMax, height);

    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, 0 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(glm::vec4)*6*length) );
    /* Upload the character data buffers to the GPU */
    glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4)*6*length)+sizeof(glm::vec2)*6*length, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(glm::vec4) * 6 * length), coords);
    glBufferSubData(GL_ARRAY_BUFFER, (sizeof(glm::vec4)*6*length), sizeof(glm::vec2)*6*length, tCoords);
    
	delete[] coords;
	delete[] tCoords;
    
    _dirty = false;
}
