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
    
    void Initialize( );  // Setup atlases and buffers
    void Terminate( void );                      // Clear atlases and delete buffers

    int AddText(std::string text, glm::vec3 position,
                bool UISpace = false, int size = 16,
                FontName font = FONT_DEFAULT, double timer = 0.0,
                Color color = COLOR_WHITE,
                glm::vec3 rotation = glm::vec3(0,0,0));
    
    void RemoveText(int blockID);
    
    void UpdateText( unsigned int blockID, std::string newText );
    
    void UpdateTextColor( unsigned int blockID, Color newColor );
    
    void UpdateTextPos( unsigned int blockID, glm::vec3 newPos );
    
    void UpdateTextRot( unsigned int blockID, glm::vec3 newRot );

    void UpdateTextTimer( unsigned int blockID, double newTimer );

    void GetTextSize( unsigned int blockID, float &width, float &height );
    std::string GetFontFileName( FontName theFont );
    
    void Update( double delta );
    void RenderLabels( void );
    void Render( TextBlock& b, FontAtlas * a );
    void RenderFixed( TextBlock& b, FontAtlas * a );
    
private:
    // FreeType library
    FT_Library ft;
    
    // Font atlases by font filename and iterator
    typedef std::map<std::string, FontAtlas*> AtlasList;
    typedef AtlasList::const_iterator AtlasIter;
    // Container for fonts
    AtlasList atlases;
    
    bool initialized;
    // The next text block ID to be generated
    unsigned int nextID = 0;
    // Counter for each frame
    int blocksRendered;
    
    // OpenGL Shader mode vars - should move somewhere else
    Shader* textShader = NULL;
    GLuint vao = 0;
    GLuint vbo = 0;
    
    FontAtlas* GetAtlas( std::string filename, int size );
    void RenderBlock( TextBlock b );
    int AddBlock( TextBlock& newBlock );
};

#endif
