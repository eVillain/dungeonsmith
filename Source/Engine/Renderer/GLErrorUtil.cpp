//
//  GLErrorUtil.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 07/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include <iostream>
#include <string>
#include "GLErrorUtil.h"

#if defined	(_WIN32) || defined (_WIN64)
#  include <GL/glew.h>
#elif __APPLE__
#  include <OpenGL/gl3.h>
#else
#  include <GL3/gl3.h>
#endif

void _check_gl_error(const char *file, int line) {
    GLenum err (glGetError());
    
    while(err!=GL_NO_ERROR) {
        std::string error;
        
        switch(err) {
            case GL_INVALID_OPERATION:
                error="INVALID_OPERATION";
                break;
            case GL_INVALID_ENUM:
                error="INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error="INVALID_VALUE";
                break;
            case GL_OUT_OF_MEMORY:
                error="OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error="INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
        
        std::cerr << "GL_" << error.c_str() <<" - "<<file<<":"<<line<<std::endl;
        err=glGetError();
    }
}
