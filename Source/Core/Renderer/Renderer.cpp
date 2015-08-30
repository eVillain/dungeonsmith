//
//  Renderer.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 19/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//
#include "Locator.h"
#include "Renderer.h"
#include "Shader.h"
#include "GLErrorUtil.h"
#include "Instanced.h"
#include "Primitives2D.h"
#include "Primitives3D.h"
#include "TextureManager.h"
#include "Mesh.h"
#include "PostProcess.h"
#include "LightSystem3D.h"
#include "StencilUtils.h"
#include "RenderUtils.h"

#include "CommandProcessor.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

const int Default_Window_Width = 1280;
const int Default_Window_Height = 720;

Renderer::Renderer() :
_window(nullptr),
_camera(nullptr)
{
    SetDefaults();
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
        printf("[Renderer] Error SDL_Init_Everything failed!\n");
        return false;
    }
    //Use OpenGL 3.2 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    
    // Create a window
    _window = SDL_CreateWindow("DungeonSmith v0.0a",       // window title
                              SDL_WINDOWPOS_CENTERED,     // x position, centered
                              SDL_WINDOWPOS_CENTERED,     // y position, centered
                              Default_Window_Width,                        // width, in pixels
                              Default_Window_Height,                        // height, in pixels
                              SDL_WINDOW_OPENGL           // flags
                              );
    
    
    //Create context
    SDL_GLContext context = SDL_GL_CreateContext( _window );
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
        glGetError();   // Ignore one bad ENUM in GLEW initialization

		const GLubyte * version = glGetString(GL_VERSION);
		printf("[Renderer] OpenGL Version:%s\n", version);

		GLint major = 0;
		GLint minor = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		if (major < 3 ||
			(major == 3 && minor < 2))
		{
			printf("[Renderer] Error could not get a modern OpenGL context!\n");
			return false;
		}

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
        
        // Add our renderer commands to the command processor
        AddCommands();
    }
    
    printf("[Renderer] Created OpenGL context.\n");
    printf("[Renderer] Pixel format: %s\n", SDL_GetPixelFormatName(SDL_GetWindowPixelFormat(_window)));
    CHECK_GL_ERROR();

    return success;
}


bool Renderer::Terminate()
{
    //Success flag
    bool success = true;
    
    success = TerminateComponents();
    
    // clean up
    if ( _window )
    {
        SDL_DestroyWindow(_window);
        _window = NULL;
    }
    SDL_Quit();
    
    return success;
}

void Renderer::SetDefaults()
{
    _debugMode = Debug_Render_Off;
    _enableLighting = true;
    _enableAA = false;
    _enableWireFrame = false;
}

bool Renderer::InitializeComponents()
{
    bool success = true;
    
    int windowWidth, windowHeight;
    SDL_GetWindowSize(_window, &windowWidth, &windowHeight);
    _gbuffer.Initialize(windowWidth, windowHeight);
    
    _instanced = new Instanced();
    _instanced->Initialize();
    _components.push_back(_instanced);
    
    _primitives2D = new Primitives2D();
    _primitives2D->Initialize();
    _components.push_back(_primitives2D);

    
    _primitives3D = new Primitives3D();
    _primitives3D->Initialize();
    _components.push_back(_primitives3D);

    _mesh = new class Mesh();
    _mesh->Initialize();
    _components.push_back(_mesh);

    _postProcess = new PostProcess();
    _postProcess->Initialize();
    _components.push_back(_postProcess);

    _lighting = new LightSystem3D();
    _lighting->Initialize();
    _components.push_back(_lighting);

    _textureManager = new TextureManager();
    
    // Generate final image framebuffer
    _textureFinal = RenderUtils::GenerateTextureRGBAF(windowWidth, windowHeight);
    glGenFramebuffers(1, &_fboFinal);
    glBindFramebuffer(GL_FRAMEBUFFER, _fboFinal);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _textureFinal, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, _gbuffer.GetDepth(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return success;
}

bool Renderer::TerminateComponents()
{
    bool success = true;
    
    for (IRenderComponent* component : _components) {
        component->Terminate();
        delete component;
    }
    _components.clear();
    
    _instanced = nullptr;
    _primitives2D = nullptr;
    _primitives3D = nullptr;
    _mesh = nullptr;
    _postProcess = nullptr;
    _lighting = nullptr;
    
    delete _textureManager;
    _textureManager = nullptr;
    
    return success;
}

void Renderer::AddCommands()
{
    CommandProcessor::AddCommand("lights", Command<>([&](){ _enableLighting = !_enableLighting; }));
    CommandProcessor::AddCommand("wires", Command<>([&](){ _enableWireFrame = !_enableWireFrame; }));
    CommandProcessor::AddCommand("aa", Command<>([&](){ _enableAA = !_enableAA; }));
    CommandProcessor::AddCommand("debugdiffuse", Command<>([&](){
        if ( _debugMode == Debug_Render_Diffuse ) { _debugMode = Debug_Render_Off; }
        else { _debugMode = Debug_Render_Diffuse; }
    }));
    CommandProcessor::AddCommand("debugspecular", Command<>([&](){
        if ( _debugMode == Debug_Render_Specular ) { _debugMode = Debug_Render_Off; }
        else { _debugMode = Debug_Render_Specular; }
    }));
    CommandProcessor::AddCommand("debugnormal", Command<>([&](){
        if ( _debugMode == Debug_Render_Normal ) { _debugMode = Debug_Render_Off; }
        else { _debugMode = Debug_Render_Normal; }
    }));
    CommandProcessor::AddCommand("debugdepth", Command<>([&](){
        if ( _debugMode == Debug_Render_Depth ) { _debugMode = Debug_Render_Off; }
        else { _debugMode = Debug_Render_Depth; }
    }));
    CommandProcessor::AddCommand("debugstencil", Command<>([&]() {
        if ( _debugMode == Debug_Render_Stencil ) { _debugMode = Debug_Render_Off; }
        else { _debugMode = Debug_Render_Stencil; }
    }));
    CommandProcessor::AddCommand("debugoff", Command<>([&](){ _debugMode = Debug_Render_Off; }));
    CommandProcessor::AddCommand("nodebug", Command<>([&](){ _debugMode = Debug_Render_Off; }));
}

void Renderer::BeginDraw()
{
    // Enable back face culling
    glCullFace( GL_BACK );
    glEnable( GL_CULL_FACE );

    // Disable all smoothing, if we want anti-aliasing we will do it in a shader
    glDisable( GL_MULTISAMPLE_ARB );
    glDisable( GL_LINE_SMOOTH );

    // Enable blending - mode Normal TODO: Set blend modes in a utility class
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // Enable depth test and write to depth buffer
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glDepthMask( GL_TRUE );

    if ( _enableWireFrame ) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
    
    // Clear color, depth and stencil
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClearDepth( 1.0 );
    glClearStencil( Stencil::Layer_Clear );
    
    // Bind default framebuffer and clear it
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    // Bind final image framebuffer and clear it
    glBindFramebuffer(GL_FRAMEBUFFER, _fboFinal);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    // Bind our G-Buffer and clear it, left on for deferred rendering
    _gbuffer.Bind();
    _gbuffer.Clear();
}

void Renderer::FinishDeferred()
{
    // Bind frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, _fboFinal);
    
    // Make sure we are rendering properly at this stage :)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    switch (_debugMode)
    {
        case Debug_Render_Diffuse:
        {
            _postProcess->TextureFullScreen(_gbuffer.GetDiffuse());
        } break;
        case Debug_Render_Specular:
        {
            _postProcess->TextureFullScreen(_gbuffer.GetSpecular());
        } break;
        case Debug_Render_Normal:
        {
            _postProcess->TextureFullScreen(_gbuffer.GetNormal());
        } break;
        case Debug_Render_Depth:
        {
            _postProcess->TextureFullScreen(_gbuffer.GetDepth());
        } break;
        case Debug_Render_Stencil:
        {
            glm::ivec2 windowSize = GetWindowSize();
            _postProcess->StencilFullScreen( windowSize  );
        } break;
        case Debug_Render_Off:
        default:
        {
                if ( _enableLighting && _camera )
                {
                    _lighting->RenderLighting(*_camera, _gbuffer);
                    glDisable( GL_BLEND );
                    Stencil::Enable();
                    // Copy over sky layer, which was not lit
                    Stencil::SetKeepEqual(Stencil::Layer_Sky);
                    _postProcess->TextureFullScreen(_gbuffer.GetDiffuse());
                    // Replace undrawn pixels with fog color
                    Stencil::SetReplaceEqual(Stencil::Layer_Clear);
                    _primitives2D->RectFilled(glm::vec2(), GetWindowSize(), COLOR_FOG_DEFAULT, 1.0);
                    _primitives2D->Render();
                    Stencil::Disable();

                }
                else    // Copy from G-buffer diffuse texture
                {
                    _postProcess->TextureFullScreen(_gbuffer.GetDiffuse());
                }
        } break;
    }
    Stencil::Enable();
    Stencil::SetReplaceLower(Stencil::Layer_Solid);
    // Draw any forward-rendered primitives
    if ( _camera ) _primitives3D->Render(_camera->GetMVP());
    Stencil::Disable();
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    if ( _enableAA )
    {
        _postProcess->ProcessFXAA(_textureFinal);
    }
    else
    {
        _postProcess->TextureFullScreen(_textureFinal);
    }
    CHECK_GL_ERROR();
    
    glEnable( GL_BLEND );
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::EndDraw()
{
    _primitives2D->Render();
    // Swap buffers, brings backbuffer to front and vice-versa
    SDL_GL_SwapWindow(_window);
}

glm::ivec2 Renderer::GetWindowSize()
{
    int windowWidth, windowHeight;
    SDL_GetWindowSize(_window, &windowWidth, &windowHeight);
    return glm::ivec2(windowWidth,windowHeight);
}

glm::vec3 Renderer::Get3DPosition(glm::ivec2 screenCoord)
{
    glm::ivec2 windowSize = GetWindowSize();
    glm::vec3 cursorPos = glm::vec3(screenCoord.x+windowSize.x/2,
                                    screenCoord.y+windowSize.y/2,
                                    0);
    // Obtain the Z position (not world coordinates but in range 0 ~ 1)
    glReadPixels(cursorPos.x, cursorPos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &cursorPos.z);

    // To grab the pixel color at cursor
    //    GLfloat col[4];
    //    glReadPixels(cursorPos.x, cursorPos.y, 1, 1, GL_RGBA, GL_FLOAT, &col);
    //    printf("cursor color:%.1f, %.1f %.1f\n", col[0],col[1],col[2]);
    
     glm::vec3 worldPosition  = glm::unProject(glm::vec3(cursorPos.x,
                                                        cursorPos.y,
                                                        cursorPos.z),
                                              _camera->GetModel(),
                                              _camera->GetProjection(),
                                              _camera->GetViewport());
    // To linearize depth
    // glm::vec2 depthParameter = glm::vec2( _camera->farDepth / ( camera->farDepth - camera->nearDepth ),
    //                                       _camera->farDepth * camera->nearDepth / ( camera->nearDepth - camera->farDepth ) );
    // float linearZ = depthParameter.y/(depthParameter.x - cursorPos.z);
    
    //    printf("Cursor depth: %f, pos: %f, %f, %f\n", linearZ, crosshairPos.x, crosshairPos.y, crosshairPos.z);

    return worldPosition;
}


