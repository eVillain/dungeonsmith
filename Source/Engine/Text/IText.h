//
//  IText.h
//  DungeonSmith
//
//  Created by The Drudgerist on 26/01/15.
//  Copyright (c) 2015 BitWiseOperators. All rights reserved.
//

#ifndef NGN_ITEXT_H
#define NGN_ITEXT_H

#include <string>
#include "RenderDefines.h"
#include "Color.h"
#include "TextConstants.h"

class FontAtlas;
class TextLabel;

class IText {
    friend class TextLabel;

public:
    virtual ~IText() {};
    
    virtual void Initialize() = 0;
    virtual void Terminate() = 0;
    
    virtual void Update( double delta ) = 0;
    virtual void Render2D() = 0;
    virtual void Render3D() = 0;

protected:
    virtual void Add(TextLabel*label) = 0;
    virtual void Remove(TextLabel*label) = 0;
    
    virtual FontAtlas* GetAtlas( std::string filename, int size ) = 0;
};

// Null class that does nothing
class NullText : public IText {
public:
    void Initialize( ) { /* Do nothing */ };
    void Terminate() { /* Do nothing */ };
    
    void Update( double delta ) { /* Do nothing */ };
    void Render2D() { /* Do nothing */ };
    void Render3D() { /* Do nothing */ };

protected:
    void Add(TextLabel*label) { /* Do nothing */ };
    void Remove(TextLabel*label) { /* Do nothing */ };
    
    FontAtlas* GetAtlas( std::string filename, int size ) { return NULL; };
};
#endif
