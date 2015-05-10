//**********************************************
//Singleton Texture Manager class
//For use with OpenGL and the LibPNG library
//**********************************************

#include "TextureManager.h"
#include "Texture.h"
#include "Console.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    // Start at the begginning of the texture map
    std::map<std::string, Texture*>::iterator i = textureMap.begin();
    
    // Unload the textures until the end of the texture map is found
    while(i != textureMap.end())
    {
        if ( i->second->RefCount() != 0 )
        {
            Console::Print("[TextureManager] Not unloading texture %s due to refcount %i",
                           i->first.c_str(), textureMap[i->first]->RefCount() );
        }
        else
        {
            i->second->Unload();
            delete i->second;
        }
        i++;
    }
    // Clear the texture map
    textureMap.clear();
}

Texture* TextureManager::LoadTexture( const std::string filePath,
                                     const std::string fileName,
                                     GLint wrapMethod,GLint filter,
                                     GLint level, GLint border )
{
    // If a texture with this name is already loaded just return the previously loaded one
    if( textureMap.find(fileName) != textureMap.end() ) {
        Console::Print("[TextureManager] Tried to load texture %s which was already loaded...", fileName.c_str() );
        return textureMap[fileName];
    }
     // Take the directory path for textures and append the filename
	std::string fullPath = filePath;
	fullPath.append( fileName );
    
    Texture* newTex = new Texture( fullPath, wrapMethod, filter, filter, level );
    if ( newTex->IsLoaded() ) {
        // Store the texture ID mapping
        textureMap[fileName] = newTex;
    } else {
        // Texture loading failed, delete it
        delete newTex;
        return NULL;
    }
    // Return success
    return newTex;
}

Texture* TextureManager::GetTexture( const std::string fileName )  {
    // If a texture with this name is already loaded just return it
    if( textureMap.find(fileName) != textureMap.end() ) {
        return textureMap[fileName];
    } else {
        Console::Print("[TextureManager] Can't get texture %s, wasn't loaded...", fileName.c_str() );
        return NULL;
    }
}

Texture* TextureManager::GetTextureForID( const int serialID ) {
    if ( serialID > (int)textureMap.size() || serialID < 0 ) { return NULL; }
    std::map<std::string, Texture*>::iterator i = textureMap.begin();
    std::advance(i, serialID);
    return i->second;
}

std::string TextureManager::GetName(const Texture *tex) {
    // Check if this texture is loaded
    std::map<std::string, Texture*>::iterator i = textureMap.begin();
    while( i != textureMap.end() ) {
        if (i->second == tex) { return i->first; }
        i++;
    }
    Console::Print("[TextureManager] Can't get name of texture %s, wasn't loaded...", tex->GetID() );
    return "";
}

std::string TextureManager::GetNameForID( const int serialID ) {
    if ( serialID > (int)textureMap.size() || serialID < 0 ) { return ""; }
    std::map<std::string, Texture*>::iterator i = textureMap.begin();
    std::advance(i, serialID);
    return i->first;
}

int TextureManager::GetSerialisedID ( const Texture* tex ) {
    int serialID = 0;
    std::map<std::string, Texture*>::iterator i = textureMap.begin();
    while( i != textureMap.end() ) {
        if ( i->second == tex ) { return serialID; }
        i++;
        serialID++;
    }
    return -1;
}

int TextureManager::GetSerialisedID ( const std::string fileName ) {
    int serialID = 0;
    std::map<std::string, Texture*>::iterator i = textureMap.begin();
    while( i != textureMap.end() ) {
        if (i->first == fileName) { return serialID; }
        i++;
        serialID++;
    }
    return -1;
}

bool TextureManager::UnloadTexture( const std::string fileName ) {
    bool result(true);
    if( textureMap.find(fileName) != textureMap.end() ) {
        // If this texture ID is already mapped
        if ( textureMap[fileName]->RefCount() == 0 ) {
            // unload it's texture, and remove it from the map
            textureMap[fileName]->Unload();
            printf("[TextureMan] Releasing texture %s\n", fileName.c_str() );
            delete textureMap[fileName];
            textureMap.erase(fileName);
        } else {
            printf("[TextureMan]Not unloading texture %s yet, refcount: %i\n",
                   fileName.c_str(),
                   textureMap[fileName]->RefCount());
        }
    } else {
        // Otherwise unload failed
        result = false;
    }
    return result;
}

bool TextureManager::UnloadTexture( Texture* tex ) {
    std::map<std::string, Texture*>::iterator i = textureMap.begin();
    while( i != textureMap.end() ) {
        if ( i->second == tex ) {
            if ( textureMap[i->first]->RefCount() == 0 ) {
                textureMap[i->first]->Unload();
                printf("[TextureMan] Releasing texture %s\n", i->first.c_str() );
				                textureMap.erase(i->first);
                delete tex;
                return true;
            } else {
                printf("[TextureMan] Not releasing texture %s yet, refcount: %i\n",i->first.c_str(), textureMap[i->first]->RefCount());
            }
        }
        i++;
    }
    return false;
}

bool TextureManager::BindTexture( const std::string fileName ) {
    bool result(true);
    // If this texture ID is mapped, bind it's texture as current
    if( textureMap.find(fileName) != textureMap.end() )
    { textureMap[fileName]->Bind(); }
    // Otherwise binding failed
    else { result = false; }
    return result;
}

void TextureManager::UnloadTextures() {
    Console::Print("[TextureManager] Unloading %i textures", textureMap.size() );
    // Start at the begginning of the texture map
    std::map<std::string, Texture*>::iterator i = textureMap.begin();
    
    // First unload the textures
    while(i != textureMap.end()) {
        i->second->Unload();
        i++;
    }
}

void TextureManager::ReloadTextures() {
    std::map<std::string, Texture*>::iterator i = textureMap.begin();

    // Then reload the textures
    while(i != textureMap.end()) {
        // Take the directory path for textures and append the filename
        std::string fullPath = i->second->GetPath();
        fullPath.append( i->first );
        i->second->LoadFromFile( fullPath );
        i++;
    }
    Console::Print("[TextureManager] Reloaded %i textures", textureMap.size() );
}
