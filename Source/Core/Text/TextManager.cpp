//
//  TextManager.cpp
//  Bloxelizer
//
//  Created by The Drudgerist on 9/4/12.
//  Copyright (c) 2012 The Drudgerist. All rights reserved.
//

#include <string>

#include "TextManager.h"
#include "TextDefaults.h"
#include "HyperVisor.h"
#include "Locator.h"
#include "IRenderer.h"
#include "FileUtil.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>     // glm::translate, glm::rotate, glm::scale

// Vector of text blocks to render
typedef std::vector<TextBlock> TextBlockVector;
// Container for text labels
static TextBlockVector textLabels;

TextManager::TextManager() :
initialized(false),
nextID(0),
blocksRendered(0) {
    textShader = NULL;
    vao = 0;
    vbo = 0;
}

TextManager::~TextManager() {
    textLabels.clear();         // Empty out all blocks
    Terminate();                // Un-initialize
}

void TextManager::Initialize() {
    if ( initialized ) { return; }
    
    textShader = new Shader();
    textShader->InitFromSource(text_vertex_shader32, text_frag_shader32);
    if ( textShader->GetProgram() == 0 ) {
        printf("[TextMan] failed to load text shader program\n");
        return;
    }
    
    // Create vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // Create the vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    glBindVertexArray(0);
    
    
    // Initialize FreeType library
    if( FT_Init_FreeType(&ft) != 0 ) {
        printf("[TextMan] Could not init FreeType library\n");
    }
    initialized = true;
}

void TextManager::Terminate() {
    if ( !initialized ) return;
    
    AtlasIter atl;
    for ( atl = atlases.begin(); atl != atlases.end(); atl++ ) {
        delete atl->second;
    }
    atlases.clear();
    FT_Done_FreeType(ft);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    delete textShader;
    textShader = NULL;
    initialized = false;
}

void TextManager::Update( double delta ) {
    TextBlockVector::iterator it=textLabels.begin();
    while ( it != textLabels.end() ) {
        if ( it->timer != 0.0 ) {
            if ( it->timer <= delta ) {
                // label timer over, remove
                textLabels.erase(it);
                continue;
            } else if ( it->timer > delta ) {
                if ( it->timer <= TEXT_FADE_TIME ) {
                    // fade out label
                    it->color.a = (GLfloat)(it->timer/TEXT_FADE_TIME);
                }
                it->timer -= delta;
            }
        }
        it++;
    }
}

int TextManager::AddText( std::string text, glm::vec3 position, bool UISpace, int size,
                         FontName font, double timer, Color color, glm::vec3 rotation ) {
    TextBlock newBlock = {position, rotation, color, size, font, 0, timer, UISpace, text};
    return AddBlock(newBlock);
}
void TextManager::RemoveText( int blockID ) {
    for ( unsigned int i=0; i < textLabels.size(); i++ ) {
        if ( textLabels[i].blockID == blockID ) {
            textLabels.erase(textLabels.begin()+i);
            return;
        }
    }
}
void TextManager::UpdateText( unsigned int blockID, std::string newText ) {
    for ( unsigned int i=0; i < textLabels.size(); i++ ) {
        if ( textLabels[i].blockID == blockID ) {
            textLabels[i].text = newText;
            return;
        }
    }
}
void TextManager::UpdateTextColor( unsigned int blockID, Color newColor ) {
    for ( unsigned int i=0; i < textLabels.size(); i++ ) {
        if (textLabels[i].blockID == blockID) {
            textLabels[i].color = newColor;
            return;
        }
    }
}
void TextManager::UpdateTextPos( unsigned int blockID, glm::vec3 newPos ) {
    for ( unsigned int i=0; i < textLabels.size(); i++ ) {
        if (textLabels[i].blockID == blockID) {
            textLabels[i].pos = newPos;
            return;
        }
    }
}
void TextManager::UpdateTextRot( unsigned int blockID, glm::vec3 newRot ) {
    for ( unsigned int i=0; i < textLabels.size(); i++ ) {
        if (textLabels[i].blockID == blockID) {
            textLabels[i].rot = newRot;
            return;
        }
    }
}
void TextManager::UpdateTextTimer( unsigned int blockID, double newTimer ) {
    for ( unsigned int i=0; i < textLabels.size(); i++ ) {
        if (textLabels[i].blockID == blockID) {
            textLabels[i].timer = newTimer;
            return;
        }
    }
}
void TextManager::GetTextSize( unsigned int blockID, float &width, float &height ) {
    for ( unsigned int i=0; i < textLabels.size(); i++ ) {
        if ( textLabels[i].blockID == blockID ) {
            std::string fontFile = GetFontFileName( textLabels[i].font );
            FontAtlas* a = GetAtlas(fontFile, textLabels[i].size);
            GlyphInfo* g = a->GetGlyphInfo();
            const uint8_t *p;
            /* Loop through all characters */
            for (p = (const uint8_t *)textLabels[i].text.c_str(); *p; p++) {
                width += g[*p].ax;
                float h = g[*p].bh;
                if ( height < h ) height = h;
            }
        }
    }
}

// TODO: MAKE THIS STATIC AND MOVE TO UTILS CLASS
std::string TextManager::GetFontFileName( FontName theFont ) {
    std::string fontFile;
    if ( theFont == FONT_DEFAULT ) {
        fontFile = ("ClearSans-Regular.ttf");
    } else if ( theFont == FONT_MENU ) {
        fontFile = ("HiLoDeco.ttf");
    } else if ( theFont == FONT_PIXEL ) {
        fontFile = ("ORANGEKI.ttf");
    } else if ( theFont == FONT_FANCY ) {
        fontFile = ("CRETINO.ttf");
    } else if ( theFont == FONT_JURA ) {
        fontFile = ("Jura-Book.ttf");
    } else if ( theFont == FONT_SEGMENT ) {
        fontFile = ("Segment14.otf");
    } else if ( theFont == FONT_FELL_NORMAL ) {
        fontFile = ("IMFeENrm29P.ttf");
    } else if ( theFont == FONT_FELL_CAPS ) {
        fontFile = ("IMFeNsc29P.ttf");
    } else if ( theFont == FONT_BENTHAM ) {
        fontFile = ("Bentham.ttf");
    } else {
        fontFile = "NO_FONT";
    }
    return fontFile;
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

void TextManager::RenderLabels( void ) {
    if ( !initialized ) return;
    
    blocksRendered = 0;
    /* Enable blending, necessary for our alpha texture */
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Bind our VAO and VBO
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    for ( unsigned int i=0; i < textLabels.size(); i++ ) {
        TextBlock* label = &textLabels[i];
        RenderBlock( *label );
        blocksRendered++;
    }
    glBindVertexArray(0);
}

void TextManager::RenderBlock( TextBlock b ) {
    
    std::string fontFile = GetFontFileName(b.font);
    FontAtlas* fAtlas = GetAtlas(fontFile, b.size);
    if ( !fAtlas ) return;
    
    float width = 0.0f;
    float height = 0.0f;
    GetTextSize(b.blockID, width, height);
    
    glm::mat4 mvp;
    if ( b.isUIText ) {
        Locator::getRenderer().GetUIMatrix(mvp);
    } else {
//        Locator::getRenderer().GetGameMatrix(mvp);
        float r_scale = 1.0f/(b.size*4.0f);
        width *= r_scale;
        height *= r_scale;
        mvp = glm::translate(mvp, glm::vec3(b.pos.x+width*0.5f, b.pos.y+height*0.5f, b.pos.z) );
        mvp = glm::rotate(mvp, b.rot.y, glm::vec3(0.0, 1.0, 0.0) );
        mvp = glm::rotate(mvp, b.rot.x, glm::vec3(1.0, 0.0, 0.0) );
        mvp = glm::rotate(mvp, b.rot.z, glm::vec3(0.0, 0.0, 1.0) );
        mvp = glm::scale(mvp, glm::vec3(r_scale));
    }

    textShader->Begin();
    // Pass MVP to shader
    textShader->setUniformM4fv("MVP", mvp);
    // Pass color to shader
    textShader->setUniform4fv("color", b.color);
    /* Use the texture containing the atlas */
    glBindTexture( GL_TEXTURE_2D, fAtlas->GetTextureID() );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    Render(b, fAtlas);
    textShader->End();
}
/**
 * Render text using the currently loaded font and currently set font size.
 */
void TextManager::Render(TextBlock& b, FontAtlas * a) {
	const uint8_t *p;
	unsigned long length = b.text.length();
	glm::vec4* coords = new glm::vec4[6 * length];
    glm::vec2* tCoords = new glm::vec2[6 * length];
    int c = 0;
    GlyphInfo* g = a->GetGlyphInfo();
    float x = b.pos.x;
    float y = b.pos.y;
    float z = b.pos.z;
    
    /* Loop through all characters */
    for (p = (const uint8_t *)b.text.c_str(); *p; p++) {
        // TODO:: NEWLINE CHARACTER USAGE
        /*if ( strncmp((const char*)p, "\n", 2) == 0) {
         printf("[TEXT RENDERER] WANTS NEW LINE SWAP\n");
         y += g[*p].bh;
         p++;
         continue;
         }*/
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
            continue;
        int aw = a->GetWidth();
        int ah = a->GetHeight();
        
        tCoords[c] = glm::vec2(g[*p].tx + g[*p].bw / aw, g[*p].ty);
		coords[c++] = glm::vec4( x2+w, -y2, z, 1.0f );
		tCoords[c] = glm::vec2(g[*p].tx, g[*p].ty);
		coords[c++] = glm::vec4( x2, -y2, z, 1.0f );
        tCoords[c] = glm::vec2(g[*p].tx, g[*p].ty + g[*p].bh / ah);
		coords[c++] = glm::vec4( x2, -y2-h, z, 1.0f );
        tCoords[c] = glm::vec2(g[*p].tx + g[*p].bw / aw, g[*p].ty);
		coords[c++] = glm::vec4( x2+w, -y2, z, 1.0f );
        tCoords[c] = glm::vec2(g[*p].tx, g[*p].ty + g[*p].bh / ah);
		coords[c++] = glm::vec4( x2, -y2-h, z, 1.0f );
        tCoords[c] = glm::vec2(g[*p].tx + g[*p].bw / aw, g[*p].ty + g[*p].bh / ah);
		coords[c++] = glm::vec4( x2+w, -y2-h, z, 1.0f );
    }
    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, 0 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(glm::vec4)*6*length) );
    /* Draw all the characters on the screen in one go */
    glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec4)*6*length)+sizeof(glm::vec2)*6*length, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(glm::vec4) * 6 * length), coords);
    glBufferSubData(GL_ARRAY_BUFFER, (sizeof(glm::vec4)*6*length), sizeof(glm::vec2)*6*length, tCoords);
    glDrawArrays(GL_TRIANGLES, 0, c);
    
	delete[] coords;
	delete[] tCoords;
}

// Adds a new label and returns its index ID
int TextManager::AddBlock( TextBlock& newBlock ) {
    newBlock.blockID = nextID++;
    textLabels.push_back(newBlock);
    return newBlock.blockID;
}
