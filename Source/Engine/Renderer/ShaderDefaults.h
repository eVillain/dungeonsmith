//
//  ShaderDefaults.h
//  DungeonSmith
//
//  Created by The Drudgerist on 02/02/15.
//  Copyright (c) 2015 BitWiseOperators. All rights reserved.
//

#ifndef DungeonSmith_ShaderDefaults_h
#define DungeonSmith_ShaderDefaults_h

#include <string>

enum ShaderType {
    Shader_Default_Vertex_Color = 0,
    Shader_Default_Uniform_Color = 1,
    Shader_Default_Texture_Uniform_Color = 2,
    Shader_Default_Sprite = 3,
    Shader_Default_Instance = 4,
};

/**********************************************************************
 * Default shader programs
 *********************************************************************/

const GLchar *default_vertex_shader[] = {
    "#version 400\n"
    "layout(location = 0) in vec4 vCoord;\n"
    "layout(location = 1) in vec4 vColor;\n"
    "out vec4 fragmentColor;\n"
    "uniform mat4 MVP;\n"
    "void main()\n"
    "{ gl_Position = MVP * vCoord;\n"
    " fragmentColor = vColor; }"
};
const GLchar *default_frag_shader[] = {
    "#version 400\n"
    "in vec4 fragmentColor;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{ color = fragmentColor; }"
};


#endif
