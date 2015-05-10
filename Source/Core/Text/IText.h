//
//  IText.h
//  Ingenium
//
//  Created by The Drudgerist on 26/01/15.
//  Copyright (c) 2015 BitWiseOperators. All rights reserved.
//

#ifndef NGN_ITEXT_H
#define NGN_ITEXT_H

#include <string>
#include <glm/glm.hpp>
#include "Color.h"

// Supported fonts: Hard-wired for now, must make these configurable later
enum FontName {
    FONT_DEFAULT,
    FONT_MENU,
    FONT_PIXEL,
    FONT_FANCY,
    FONT_JURA,
    FONT_SEGMENT,
    FONT_FELL_NORMAL,
    FONT_FELL_CAPS,
    FONT_BENTHAM,
};

// Data structure for text in 3D space
typedef struct {
    glm::vec3 pos;
    glm::vec3 rot;
    Color color;
    int size;
    FontName font;
    int blockID;
    double timer;
    bool isUIText;
    std::string text;
} TextBlock;

class IText {
public:
    virtual ~IText() {};
    virtual void Initialize( ) = 0;
    virtual void Terminate( void ) = 0;
    
    // TODO: Make this class so this function returns the TextBlock object
    // which you can just manipulate directly, eliminating the need for the setter functions below
    virtual int AddText(std::string text, glm::vec3 position,
                bool UISpace = false, int size = 16,
                FontName font = FONT_DEFAULT, double timer = 0.0,
                Color color = COLOR_WHITE,
                glm::vec3 rotation = glm::vec3(0,0,0)) = 0;
    
    virtual void RemoveText(int blockID) = 0;
    
    virtual void UpdateText( unsigned int blockID, std::string newText ) = 0;
    
    virtual void UpdateTextColor( unsigned int blockID, Color newColor ) = 0;
    
    virtual void UpdateTextPos( unsigned int blockID, glm::vec3 newPos ) = 0;
    
    virtual void UpdateTextRot( unsigned int blockID, glm::vec3 newRot ) = 0;
    
    virtual void UpdateTextTimer( unsigned int blockID, double newTimer ) = 0;
    
    virtual void GetTextSize( unsigned int blockID, float &width, float &height ) = 0;
    virtual std::string GetFontFileName( FontName theFont ) = 0;
    
    virtual void Update( double delta ) = 0;
    virtual void RenderLabels( void ) = 0;
};

// Null class that does nothing
class NullText : public IText {
public:
    void Initialize( ) { /* Do nothing */ };
    void Terminate( void ) { /* Do nothing */ };
    
    int AddText(std::string text, glm::vec3 position,
                        bool UISpace = false, int size = 16,
                        FontName font = FONT_DEFAULT, double timer = 0.0,
                        Color color = COLOR_WHITE,
                glm::vec3 rotation = glm::vec3(0,0,0)) { /* Do nothing */ return -1; };
    
    void RemoveText(int blockID) { /* Do nothing */ };
    
    void UpdateText( unsigned int blockID, std::string newText ) { /* Do nothing */ };
    
    void UpdateTextColor( unsigned int blockID, Color newColor ) { /* Do nothing */ };
    
    void UpdateTextPos( unsigned int blockID, glm::vec3 newPos ) { /* Do nothing */ };
    
    void UpdateTextRot( unsigned int blockID, glm::vec3 newRot ) { /* Do nothing */ };
    
    void UpdateTextTimer( unsigned int blockID, double newTimer ) { /* Do nothing */ };
    
    void GetTextSize( unsigned int blockID, float &width, float &height ) { /* Do nothing */ };
    std::string GetFontFileName( FontName theFont ) { /* Do nothing */ return ""; };
    
    void Update( double delta ) { /* Do nothing */ };
    void RenderLabels( void ) { /* Do nothing */ };
};
#endif
