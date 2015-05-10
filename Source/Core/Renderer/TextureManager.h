//**********************************************
// Texture Manager class
// Uses LibPNG to load .png files
// For use with OpenGL and the LibPNG library
//**********************************************

#ifndef NE_TEXTUREMANAGER_H
#define NE_TEXTUREMANAGER_H

#include <map>
#include <string>
#include "Texture.h"

class TextureManager {
public:
    TextureManager();
    ~TextureManager();
    
    // Load a texture an make it the current texture
    Texture* LoadTexture(const std::string filePath,
                         const std::string fileName,    // Texture file name, will serve as reference
                         GLint wrapMethod = GL_REPEAT,  // Texture wrapping method
                         GLint filter = GL_NEAREST,     // Texture filtering (mag and min for now)
                         GLint level = 0,               // Texture mipmapping level
                         GLint border = 0);             // Texture border size
    
    Texture* GetTexture( const std::string fileName );
    Texture* GetTextureForID( const int serialID );
    std::string GetName( const Texture* tex );
    std::string GetNameForID( const int serialID );
    int GetSerialisedID( const Texture* tex );
    int GetSerialisedID( const std::string fileName );

    // Free the memory for a texture
    bool UnloadTexture( const std::string fileName );
    bool UnloadTexture( Texture* tex );

    // Set the current texture
    bool BindTexture( const std::string fileName );

    // Free all texture memory
    void UnloadAllTextures( void );
    void UnloadTextures( void );
    
    // Reload all the textures
    void ReloadTextures ( void );
    
protected:
    std::map<std::string, Texture*> textureMap;                 // Map of loaded textures, by name
};

#endif