//
//  Renderer.cpp
//  DungeonSmith
//
//  Created by eVillain on 19/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

//#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

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
        //Initialize GLEW
        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        if( glewError != GLEW_OK )
        {
            printf( "[Renderer] Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
        }
        
        //Use Vsync
        if( SDL_GL_SetSwapInterval( 1 ) < 0 )
        {
            printf( "[Renderer] Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
        }
        
        //Initialize OpenGL
        if( !InitializeGL() )
        {
            printf( "[Renderer] Unable to initialize OpenGL!\n" );
            success = false;
        }
    }
    
    const GLubyte * version = glGetString(GL_VERSION);
    printf("[Renderer] OpenGL Version:%s\n", version);

    return success;
}


bool Renderer::Terminate()
{
    //Success flag
    bool success = true;
    
    // clean up
    if ( window )
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
    
    return success;
}

void Renderer::BeginDraw()
{
    //Clear color buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndDraw()
{
    // Swap buffers, brings backbuffer to front and vice-versa
    SDL_GL_SwapWindow(window);
}

bool Renderer::InitializeGL()
{
    //Success flag
    bool success = true;
    
    return success;
}

// Matrix functionality
void Renderer::GetUIMatrix( glm::mat4& target ) {
    int windowWidth = 640;
    int windowHeight = 480;
    // 2D projection with origin (0,0) as center of window
    GLfloat hw = windowWidth*0.5f;
    GLfloat hh = windowHeight*0.5f;
    target = glm::ortho<GLfloat>(-hw, hw, -hh, hh, UI_NEARDEPTH, UI_FARDEPTH);
}

