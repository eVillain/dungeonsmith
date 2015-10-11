//
//  ShaderFactory.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 9/7/12.
//  Copyright (c) 2012 The Drudgerist. All rights reserved.
//

#include "ShaderFactory.h"
#include "FileUtil.h"
#include "ShaderDefaults.h"

ShaderFactory::ShaderList ShaderFactory::shaders;

Shader* ShaderFactory::LoadFromFile( const std::string fshPath,
                                     const std::string vshPath,
                                     const std::string gshPath )
{
    Shader* shader = new Shader();
    
    std::string vertShader = FileUtil::GetPath().append("Shaders/");
    vertShader.append(vshPath);
    std::string fragShader = FileUtil::GetPath().append("Shaders/");
    fragShader.append(fshPath);
    
    if ( gshPath.length() )
    {
        std::string geomShader = FileUtil::GetPath().append("Shaders/");
        geomShader.append(gshPath);
        
        shader->InitFromFile( geomShader, vertShader, fragShader );
    }
    else
    {
        shader->InitFromFile( vertShader, fragShader );
    }

    if (shader->GetProgram() == 0)
    {
        printf("[ShaderFactory] Shader program loading failed, loading default\n");
        shader->InitFromSource( default_vertex_shader, default_frag_shader );
    }
    shaders.push_back(shader);
    return shader;
}

void ShaderFactory::ClearShader( Shader* shader )
{
    ShaderList::iterator it = std::find(shaders.begin(), shaders.end(), shader);
    if ( it != shaders.end() )
    {
        delete *it;
        shaders.erase(it);
    }
}

void ShaderFactory::ClearShaders()
{
    if ( shaders.size() )
    {
        printf("[ShaderFactory] Releasing shaders...\n");
        for( unsigned int i=0; i< shaders.size(); i++ )
        {
            delete shaders[i];
        }
        shaders.clear();
    }
}
