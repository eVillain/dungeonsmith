//
//  ParticleEditor.h
//  Ingenium
//
//  Created by The Drudgerist on 05/03/14.
//  Copyright (c) 2014 The Drudgerist. All rights reserved.
//

#ifndef NGN_PARTICLE3D_EDITOR_H
#define NGN_PARTICLE3D_EDITOR_H

#include "Scene.h"
#include "InputListener.h"
#include "ParticleSys.h"
#include "Camera.h"

class GUIButton;
class GUIMenu;
class GUIFileMenu;

class ParticleEditor : public Scene, public InputEventListener, public MouseEventListener
{
public:
    ParticleEditor();
    ~ParticleEditor();
    
    // Overridden from Scene
    void Initialize( void );
    void ReInitialize( void );
    void Release( void );
    void Pause( void );
    void Resume( void );
    void Update ( double delta );
    void Draw( void );

    void ShowEditor( void );
    void RemoveEditor( void );
    
    float timeScaler;
    
private:
    bool OnEvent( const std::string& theEvent, const float& amount );
    bool OnMouse( const glm::ivec2& coord );

    Camera camera;

    
    std::vector<GUIButton*> buttonVect;
    GUIMenu* fileMenu;                      // New/Load/Save/Quit
    GUIMenu* particleMenu;                  // Particle system vars
    GUIFileMenu* fileSelectMenu;            // Open file menu
    GUIMenu* optionsMenu;                   // Engine options menu
    GUIMenu* cameraMenu;                    // Camera menu
    GUIButton *optionsBtn, *cameraBtn;

    ParticleSys* m_particleSys;
    
    
    glm::vec2 joyMoveInput;
    glm::vec2 joyRotateInput;
    void UpdateMovement();
    
    // Editor button callbacks
    void LoadSystemButtonCB( void*data );
    void CloseEditorButtonCB( void*data );
    void OpenOptionsButtonCB( void*data );
    
    void ShowOptionsMenu();
    void RemoveOptionsMenu();
    
    void LoadSystem( const std::string fileName );
    void SaveSystem( const std::string fileName );

};

#endif
