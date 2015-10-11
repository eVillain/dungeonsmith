//
//  ShaderFactory.h
//  Bloxelizer
//
//  Created by The Drudgerist on 9/7/12.
//  Copyright (c) 2012 The Drudgerist. All rights reserved.
//

#ifndef Bloxelizer_ShaderFactory_h
#define Bloxelizer_ShaderFactory_h

#include <map>
#include <vector>
#include <string>
#include "Shader.h"

class ShaderFactory {
public:
    /*
     * Clears all the shaders and deletes
     * them from the list.
     */
    static void ClearShaders();
    /*
     * Loads from fragment, vertex and (optionally) geometry shader files.
     */
    static Shader* LoadFromFile(const std::string fshPath, const std::string vshPath, const std::string gshPath = "");
    /*
     * Cleans up one shader.
     */
    static void ClearShader( Shader* oldShader );
    
    /*
     * Syntactic sugar typedef for a vector of Shaders.
     */
    typedef std::vector<Shader*> ShaderList;
private:
    static ShaderList shaders;
};

#endif
