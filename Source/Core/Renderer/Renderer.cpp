//
//  Renderer.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 19/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

//#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "GLErrorUtil.h"
#include "DrawPrimitives.h"
#include "TextureManager.h"

#define UI_NEARDEPTH -100.0
#define UI_FARDEPTH 100.0

// Testing only, remove later
GLuint gVBO = 0;
GLuint gVAO = 0;

GLuint vs,fs;
GLuint shader_programme;
Shader* shader;

//Get vertex source
const char* vertex_shader_test =
{
    "#version 400\n"
    "in vec3 vp;"
    "void main () {"
    "  gl_Position = vec4 (vp, 1.0);"
    "}"
};

//Test fragment source
const char* fragment_shader_test =
{
    "#version 400\n"
    "out vec4 frag_colour;"
    "void main () {"
    "  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
    "}"
};

Renderer::Renderer() :
window(NULL)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize()
{
    //Success flag
    bool success = true;
    
    // init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("[Renderer] Error SDL_Init_Everything_OMGWTFBBQ_CALLaDEVELOPER!\n");
        return false;
    }
    //Use OpenGL 3.2 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    
    // Create a window
    window = SDL_CreateWindow("DungeonSmith v0.0a",       // window title
                              SDL_WINDOWPOS_CENTERED,     // x position, centered
                              SDL_WINDOWPOS_CENTERED,     // y position, centered
                              640,                        // width, in pixels
                              480,                        // height, in pixels
                              SDL_WINDOW_OPENGL           // flags
                              );
    
    
    //Create context
    SDL_GLContext context = SDL_GL_CreateContext( window );
    if( context == NULL )
    {
        printf( "[Renderer] OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        const GLubyte * version = glGetString(GL_VERSION);
        printf("[Renderer] OpenGL Version:%s\n", version);

        //Initialize GLEW
        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        if( glewError != GLEW_OK )
        {
            printf( "[Renderer] Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
        }
        glGetError();   // Ignore one bad ENUM in GLEW initialization

        //Use Vsync
        if( SDL_GL_SetSwapInterval( 1 ) < 0 )
        {
            printf( "[Renderer] Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
        }

        //Initialize OpenGL
        if( !InitializeComponents() )
        {
            printf( "[Renderer] Unable to initialize renderer components!\n" );
            success = false;
        }
    }

    return success;
}


bool Renderer::Terminate()
{
    //Success flag
    bool success = true;
    
    success = TerminateComponents();
    
    // clean up
    if ( window )
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
    
    return success;
}

bool Renderer::InitializeComponents()
{
    bool success = true;
    
    primitives = new DrawPrimitives(this);
    textureManager = new TextureManager();
    
    return success;
}

bool Renderer::TerminateComponents()
{
    bool success = true;
    
    delete primitives;
    primitives = NULL;
    delete textureManager;
    textureManager = NULL;
    
    return success;
}

void Renderer::BeginDraw()
{
    //Clear color buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
}

void Renderer::EndDraw()
{
    primitives->Render();
    // Swap buffers, brings backbuffer to front and vice-versa
    SDL_GL_SwapWindow(window);
}



glm::vec2 Renderer::GetWindowSize()
{
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    return glm::vec2(windowWidth,windowHeight);
}

// Matrix functionality
void Renderer::GetUIMatrix( glm::mat4& target ) {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    // 2D projection with origin (0,0) at center of window
    GLfloat hw = windowWidth*0.5f;
    GLfloat hh = windowHeight*0.5f;
    target = glm::ortho<GLfloat>(-hw, hw, -hh, hh, UI_NEARDEPTH, UI_FARDEPTH);
}

