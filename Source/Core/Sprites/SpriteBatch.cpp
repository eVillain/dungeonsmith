//
//  SpriteBatch.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 1/19/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include "SpriteBatch.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Sprite.h"
#include "Dictionary.h"
#include "IRenderer.h"
#include "Locator.h"
#include "Console.h"

SpriteBatch::SpriteBatch() :
texture(NULL)
{
}

SpriteBatch::~SpriteBatch()
{
    CleanUp();
}

void SpriteBatch::LoadFromFile( const std::string filePath, const std::string fileName ) {
    if ( texture || frames.size() ) {
        Console::Print("[SpriteBatch] Can't load batch, already loaded");
    }
    std::string dictPath = filePath;
    dictPath.append(fileName);
    Dictionary rootDict;
    if(!rootDict.loadRootSubDictFromFile( dictPath.c_str() )){ return; }
    // Get metadata from the subdict
    if(!rootDict.stepIntoSubDictWithKey("metadata")){  return; }
    texFileName = rootDict.getStringForKey("textureFileName");
    
    // Get texture
    texture = Locator::getRenderer().Textures()->LoadTexture( filePath, texFileName );
    
    if ( texture == NULL ) {
        return;
    } else {
        texture->Retain();
    }
    //Step out of the sub dict and into frames
    rootDict.stepOutOfSubDict();
    if(!rootDict.stepIntoSubDictWithKey("frames")){  return; }
    //Read an array of strings from that dict
    frameIDs = rootDict.getAllKeys();
    for ( unsigned int i=0; i < frameIDs.size(); i++ ) {
        if(!rootDict.stepIntoSubDictWithKey(frameIDs[i].c_str())) {
            Console::Print("[SpriteBatch] Error parsing frame");
            return;
        }
        Rect2D r = rootDict.getRectForKey("frame");
        frames[frameIDs[i]] = r;
        rootDict.stepOutOfSubDict();
    }
    bool debugInfo = true;
    if ( debugInfo ) {
        int format = rootDict.getIntegerForKey("format");
        int numFrames = rootDict.getNumKeys();
        Console::Print("[SpriteBatch] Loaded .plist - format %i, frame count %i", format, numFrames);
    }
}

void SpriteBatch::CleanUp() {
    if (texture != NULL) {
        texture->Release();
        Locator::getRenderer().Textures()->UnloadTexture(texture);
        texture = NULL;
    } else {
        Console::Print("[SpriteBatch] No texture to cleanup");
    }
    frames.clear();
    children.clear();
}

void SpriteBatch::AddChild( Sprite* child ) {
    if (!ContainsChild(child)) {
        children.push_back(child);
    } else {
        Console::Print("[SpriteBatch] trying to add already contained child");
    }
}

void SpriteBatch::RemoveChild( Sprite* child ) {
    for ( unsigned int i=0; i < children.size(); i++ ) {
        if ( children[i] == child ) {
            children.erase( children.begin() + i );
            return;
        }
    }
    Console::Print("[SpriteBatch] trying to remove unattached child");
}

bool SpriteBatch::ContainsChild( Sprite* child ) {
    for ( unsigned int i=0; i < children.size(); i++ ) {
        if ( children[i] == child ) {
            return true;
        }
    }
    return false;
}

Rect2D SpriteBatch::GetRectForFrame( std::string frame ) const {
    if( frames.find(frame) != frames.end() ) {
        return frames.at(frame);
    }
    return Rect2D();
}

const Rect2D SpriteBatch::GetTexRectForFrame( const std::string frame ) const {
    if( frames.find(frame) != frames.end() ) {
        Rect2D r = frames.find(frame)->second;
        const float tw = ( r.w / (texture->GetWidth()) );
        const float th = ( r.h / (texture->GetHeight()) );
        const float tx = ( r.x / (texture->GetWidth()-1) );
        const float ty = 1.0f - ( r.y / (texture->GetHeight()) ) - th;
        return Rect2D(tx,ty,tw,th);
    }
    return Rect2D();
}

Sprite* SpriteBatch::AddSpriteForFrame( const std::string frame ) {
    if( frames.find(frame) != frames.end() ) {
        Sprite * newSprite = new Sprite(glm::vec2(), this, frame);
        AddChild(newSprite);
        return newSprite;
    }
    printf("[SpriteBatch] couldn't add sprite frame %s\n", frame.c_str());
    return NULL;
}

std::string SpriteBatch::GetFrameForID( const int frameID ) {
    
    if ( (int)frameIDs.size() < frameID && frameID >= 0 ) {
        printf("[SpriteBatch] couldn't get sprite frame %i\n", frameID);
        return "";
    }
    return frameIDs[frameID];
}

const int SpriteBatch::GetIDForFrame( const std::string frame ) const {
    for (unsigned int i=0; i < frameIDs.size(); i++) {
        if ( frameIDs[i] == frame ) {
            return i;
        }
    }
    return -1;
}

std::vector<std::string> SpriteBatch::GetFrameNames( void ) {
    return frameIDs;
}
int SpriteBatch::GetNumFrames( void ) {
    return (int)frameIDs.size();
}
void SpriteBatch::SetFrameForSprite( const std::string frame, Sprite* sprite ) {
    if( frames.find(frame) != frames.end() ) {
        Rect2D r = frames[frame];
        const float tx = ( r.x / texture->GetWidth() );
        const float ty = 1.0f - ( r.y / texture->GetHeight() );
        const float tw = ( r.w / texture->GetWidth() );
        const float th = ( r.h / texture->GetHeight() );
        sprite->SetFrame(glm::vec2(r.w,r.h), Rect2D(tx,ty,tw,th), GetIDForFrame(frame));
    }
    else
    {
        // Frame not found, set null sprite
        sprite->SetFrame(glm::vec2(0,0), Rect2D(), -1);
    }
}

std::vector<Sprite*> SpriteBatch::GetSpritesAtPoint( glm::vec2 point ) {
    std::vector<Sprite*> container;
    for ( unsigned int i=0; i < children.size(); i++ ) {
        Sprite* child = children[i];
        Rect2D childArea = Rect2D( child->position.x, child->position.y, child->size.x*child->scale.x, child->size.y*child->scale.y );
        if ( childArea.Contains(point) ) {
            container.push_back(child);
        }
    }
    return container;
}
std::vector<Sprite*> SpriteBatch::GetSpritesInRect( Rect2D rect ) {
    std::vector<Sprite*> container;
    for ( unsigned int i=0; i < children.size(); i++ ) {
        Sprite* child = children[i];
        Rect2D childArea = Rect2D( child->position.x, child->position.y, child->size.x*child->scale.x, child->size.y*child->scale.y );
        if ( childArea.Intersects(rect) ) {
            container.push_back(child);
        }
    }
    return container;
}

void SpriteBatch::Update( const double dt ) {
    for ( unsigned int i=0; i < children.size(); i++ ) {
        Sprite* s = children[i];
        s->Update( dt );
    }
}

void SpriteBatch::Draw() {
//    IRenderer& renderer = Locator::getRenderer();
//    renderer->DrawSpriteBatch(*this);
    
//    texture->Bind();
//    // Bind the texture, enable the proper arrays
//    glEnableClientState( GL_VERTEX_ARRAY );
//    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
//    
//    glEnable( GL_TEXTURE_2D );
//    glEnable( GL_BLEND );

//    for ( unsigned int i=0; i < children.size(); i++ ) {
//        renderer->DrawSprite(*children[i]);
//        Sprite* s = children[i];
//        s->Visit();
//    }
//    glDisable( GL_TEXTURE_2D );
}
