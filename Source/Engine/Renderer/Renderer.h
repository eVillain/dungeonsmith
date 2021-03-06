//
//  Renderer.h
//  DungeonSmith
//
//  Created by The Drudgerist on 19/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__Renderer__
#define __DungeonSmith__Renderer__

#include "RenderDefines.h"
#include "IRenderer.h"
#include "IRenderComponent.h"
#include "Color.h"
#include "GBuffer.h"
#include <vector>

class SDL_Window;
class TextureManager;
class Instanced;
class Primitives2D;
class Primitives3D;
class Mesh;
class PostProcess;
class LightSystem3D;

typedef enum : int {
    Debug_Render_Off = 0,
    Debug_Render_Diffuse = 1,
    Debug_Render_Specular = 2,
    Debug_Render_Normal = 3,
    Debug_Render_Depth = 4,
    Debug_Render_Stencil = 5,
} DebugRenderMode;

class Renderer : public IRenderer {
public:
    Renderer();
    ~Renderer();
    
    bool Initialize();
    bool Terminate();

    // Begins drawing in a new frame - clears buffers and binds G-Buffer
    void BeginDraw();
    // Runs post-processing on G-Buffer and binds frame buffer
    void FinishDeferred();
    // Finishes drawing for this frame - swaps frame buffer
    void EndDraw();

    
    Instanced* DrawInstanced() { return _instanced; };
    Primitives2D* DrawPrimitives2D() { return _primitives2D; };
    Primitives3D* DrawPrimitives3D() { return _primitives3D; };

    Mesh* DrawMesh() { return _mesh; };
    TextureManager* Textures() { return _textureManager; };
    LightSystem3D* Lighting() { return _lighting; };
    
    void SetCamera( Camera* camera ) { _camera = camera; };
    Camera* GetCamera() { return _camera; };
    glm::ivec2 GetWindowSize();
    glm::vec3 Get3DPosition(glm::ivec2 screenCoord);
private:
    // Components are just subsystems for the renderer (ie. meshes/primitives)
    std::vector<IRenderComponent*> _components;
    bool InitializeComponents();
    bool TerminateComponents();
    
    void AddCommands();
    
    SDL_Window * _window;
    GBuffer _gbuffer;
    GLuint _fboFinal, _textureFinal;
    
    Camera* _camera;
    
    Instanced* _instanced;
    Primitives2D* _primitives2D;
    Primitives3D* _primitives3D;
    class Mesh* _mesh;
    PostProcess* _postProcess;
    LightSystem3D* _lighting;
    
    TextureManager* _textureManager;
    
    void SetDefaults();
    // Options
    bool _enableLighting;
    bool _enableAA;
    bool _enableWireFrame;
    
    DebugRenderMode _debugMode;
};



#endif /* defined(__DungeonSmith__Renderer__) */
