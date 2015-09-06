//
//  ParticleEditor.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 10/10/12.
//  Copyright (c) 2012 The Drudgerist. All rights reserved.
//
#include "ParticleEditor.h"
#include "Input.h"

#include "Console.h"

#include "GUIButton.h"
//#include "GUIFileMenu.h"
#include "TextureManager.h"
#include "IRenderer.h"
#include "LightSystem3D.h"

#include "TextManager.h"
#include "Camera.h"
#include "GUIWidget.h"
#include "Physics.h"

#include "SceneManager.h"
#include "LocalGame.h"

#include "Locator.h"
#include "Particles.h"
#include "FileUtil.h"

ParticleEditor::ParticleEditor() : Scene("Editor")
{
    Input::RegisterEventObserver(this);
    Input::RegisterMouseObserver(this);

    optionsMenu = NULL;
    fileMenu = NULL;
    fileSelectMenu = NULL;
    cameraMenu = NULL;
    cameraBtn = NULL;
    particleMenu = NULL;
    optionsBtn = NULL;
    optionsMenu = NULL;
    m_particleSys = NULL;
    timeScaler = 1.0f;
    
    camera.position = glm::vec3(16,16,16);
    camera.targetPosition = glm::vec3(16,16,16);
    camera.targetRotation = glm::vec3(-45,45,0);
    camera.rotation = glm::vec3(-45,45,0);
}
ParticleEditor::~ParticleEditor()
{
    Input::UnRegisterEventObserver(this);
    Input::UnRegisterMouseObserver(this);

    Locator::getParticles().RemoveSystem(m_particleSys);
}
void ParticleEditor::Initialize()
{
    Scene::Initialize();
    ShowEditor();
}
void ParticleEditor::ReInitialize()
{
    ShowEditor();
}
void ParticleEditor::Pause( void )
{
    if ( !IsPaused() )
    {
        Scene::Pause();
        RemoveEditor();
        Input::UnRegisterEventObserver(this);
        Input::UnRegisterMouseObserver(this);
    }

}
void ParticleEditor::Resume( void )
{
    if ( IsPaused() )
    {
        Scene::Resume();
        ShowEditor();
        Input::RegisterEventObserver(this);
        Input::RegisterMouseObserver(this);
    }
}
void ParticleEditor::Release()
{
    Scene::Release();
}
void ParticleEditor::ShowEditor()
{
    RemoveEditor();
    
    int bW = 140;       // Button width
    int bH = 24;        // Button height
    int padding = 8;    // Button padding
    int posX = padding-(Locator::getRenderer().GetWindowSize().x/2);
    int posY = Locator::getRenderer().GetWindowSize().y/2-(bH+padding);
    glm::ivec2 resolution = Locator::getRenderer().GetWindowSize();
    if ( !optionsBtn ) {
        // Options menu button
//        optionsBtn = GUIButton::CreateButton("", (resolution.x/2)-(padding+32), posY-8, 32, 32, ( [=]() {
//            if ( optionsMenu == NULL ) { this->ShowOptionsMenu(); }
//            else { this->RemoveOptionsMenu(); }
//        } ), BUTTON_TYPE_DEFAULT, true, "OptionsDefault.png", "OptionsActive.png", "OptionsPressed.png" );
    }
    if ( optionsMenu != NULL ) {
//        ButtonBase::ToggleButton((ButtonBase*)optionsBtn);
    }
    if ( !cameraBtn ) {
        // Camera menu
//        cameraBtn = UIButtonLambda::CreateButton("", (resolution.x/2)-(padding+32+32), posY-8, 32, 32, ( [=]() {
//            if ( cameraMenu == NULL ) {
//                int pX = posX+bW+8;
//                int pY = resolution.y/2-(bH+8);
//                if ( optionsMenu ) pY -= (optionsMenu->h+optionsMenu->contentHeight+8);
//                cameraMenu = new GUIMenu(pX, pY, bW, bH, "Camera");
//                Camera& camera = m_hyperVisor.GetCamera();
//                cameraMenu->AddVar<bool>("Auto Rotate", &camera.autoRotate, "Camera" );
//                cameraMenu->AddVar<bool>("3rd Person", &camera.thirdPerson, "Camera" );
//                cameraMenu->AddSlider<float>("Distance", &camera.distance, 1.0f, 20.0f, "Camera" );
//                cameraMenu->AddSlider<float>("Height", &camera.height, -20.0f, 20.0f, "Camera" );
//                cameraMenu->AddVar<bool>("Elastic", &camera.elasticMovement, "Camera" );
//                cameraMenu->AddSlider<float>("Elasticity", &camera.elasticity, 1.0f, 100.0f, "Camera" );
//                cameraMenu->AddVar<bool>("Clipping", &camera.physicsClip, "Camera" );
//                cameraMenu->AddVar<bool>("AutoFocus", &camera.autoFocus, "Camera" );
//                cameraMenu->AddSlider<float>("FOV", &camera.fieldOfView, 10.0f, 179.0f, "Camera" );
//                cameraMenu->AddSlider<float>("Near Depth", &camera.nearDepth, 0.001f, 1.0f, "Camera" );
//                cameraMenu->AddSlider<float>("Far Depth", &camera.farDepth, 10.0f, 1000.0f, "Camera" );
//                cameraMenu->AddSlider<float>("Focal Depth", &camera.focalDepth, 10.0f, 200.0f, "Camera" );
//                cameraMenu->AddSlider<float>("Focal Length", &camera.focalLength, 30.0f, 300.0f, "Camera" );
//                cameraMenu->AddButton("Close", ( [=]() {
//                    if ( cameraMenu != NULL ) {
//                        delete cameraMenu;
//                        cameraMenu = NULL;
//                    }
//                }  ) );
//                cameraMenu->Sort();
//            } else {
//                delete cameraMenu;
//                cameraMenu = NULL;
//            }
//        } ), BUTTON_TYPE_DEFAULT, true, "CameraDefault.png", "CameraActive.png", "CameraPressed.png" );
    }
//    if ( cameraMenu != NULL ) {
//        ButtonBase::ToggleButton((ButtonBase*)cameraBtn);
//    }
//    if ( !fileMenu ) {
//        // File menu
//        fileMenu = new UIMenu(posX, posY, bW, bH, "File");
//    } else {
//        fileMenu->ClearWidgets();
//    }
//    fileMenu->AddButton("Quit", (  [=]() { m_hyperVisor.GetSceneManager()->RemoveActiveScene(); }  ) );
//    if ( !m_particleSys ) {
//        fileMenu->AddButton("New", ( [=]() {
//            // New system TODO
//            ShowEditor();
//        } ) );
//    } else {
//        fileMenu->AddButton("Save", ( [=]() {
//            if ( fileSelectMenu == NULL ) {
//                fileSelectMenu = new UIFileMenu<ParticleEditor>(8+140+8, posY, 200, 20, SysCore::GetPath().append("Data/Particles/"),
//                                                                ".plist",
//                                                                "Select particle file:",
//                                                                "defaultParticle",
//                                                                false,
//                                                                this,
//                                                                &ParticleEditor::SaveSystem );
//            } else {
//                delete fileSelectMenu;
//                fileSelectMenu = NULL;
//            }
//        }  ) );
//    }
//    fileMenu->AddButton("Load", ( [=]() {
//        if ( fileSelectMenu == NULL ) {
//            fileSelectMenu = new UIFileMenu<ParticleEditor>(posX+bW+padding, posY, 200, 22, SysCore::GetPath().append("Data/Particles/"),
//                                                            ".plist",
//                                                            "Select particle file:",
//                                                            "defaultParticle",
//                                                            true,
//                                                            this,
//                                                            &ParticleEditor::LoadSystem );
//        } else {
//            delete fileSelectMenu;
//            fileSelectMenu = NULL;
//        }
//    }  ) );
//    if ( m_particleSys ) {
//        fileMenu->AddButton("Close", ( [=]() {
//            m_hyperVisor.GetParticleMan()->RemoveSystem(m_particleSys);
//            m_particleSys = NULL;
//            ShowEditor();
//        } ) );
//    }
//    posY -= fileMenu->h+fileMenu->contentHeight+padding;
//    if ( !particleMenu ) {
//        particleMenu = new UIMenu(posX,posY,200,bH,"Particle System");
//    } else {
//        particleMenu->ClearWidgets();
//    }
//    if ( m_particleSys ) {
//        if ( m_hyperVisor.GetParticleMan()->IsPaused() ) {
//            particleMenu->AddButton("Resume", ( [=]() {
//                m_hyperVisor.GetParticleMan()->Resume();
//                ShowEditor();
//            }  ) );
//        } else {
//            particleMenu->AddButton("Pause", ( [=]() {
//                m_hyperVisor.GetParticleMan()->Pause();
//                ShowEditor();
//            }  ) );
//        }
//        float posScale = 16.0f;
//        float sizeScale = 2.0f;
//        if ( m_particleSys->dimensions == ParticleSys2D ) {
//            posScale = 640.0f;
//            sizeScale = 512.0f;
//        }
//        particleMenu->AddButton("active", ( [=]() { m_particleSys->active = !m_particleSys->active; } ) );
//        particleMenu->AddVar("particleCount", &m_particleSys->particleCount);
//        particleMenu->AddVar("blendFuncSrc", &m_particleSys->blendFuncSrc);
//        particleMenu->AddVar("blendFuncDst", &m_particleSys->blendFuncDst);
//
//        particleMenu->AddSlider("emitterType", &m_particleSys->emitterType, 0, 1);
//        particleMenu->AddSlider("dimensions", &m_particleSys->dimensions, 0, 1);
//        particleMenu->AddSlider("lighting", &m_particleSys->lighting, 0, 1);
//        
//        particleMenu->AddSlider("PosX", &m_particleSys->position.x, -posScale, posScale, "Position");
//        particleMenu->AddSlider("PosY", &m_particleSys->position.y, -posScale, posScale, "Position");
//        particleMenu->AddSlider("PosZ", &m_particleSys->position.z, -posScale, posScale, "Position");
//
//        
//        particleMenu->AddSlider("maxParticles", &m_particleSys->maxParticles, 1, 10000);
//        particleMenu->AddSlider("Time Scale", &timeScaler, 0.0f, 2.0f);
//        particleMenu->AddSlider("duration", &m_particleSys->duration, -1.0f, 100.0f);
//        particleMenu->AddSlider("emissionRate", &m_particleSys->emissionRate, 0.0f, 10000.0f);
//
//        particleMenu->AddSlider("angle", &m_particleSys->angle, -360.0f, 360.0f);
//        particleMenu->AddSlider("angleVar", &m_particleSys->angleVar, -360.0f, 360.0f);
//
//        particleMenu->AddSlider("finishParticleSize", &m_particleSys->finishParticleSize, 0.0f, sizeScale);
//        particleMenu->AddSlider("finishParticleSizeVar", &m_particleSys->finishParticleSizeVar, 0.0f, sizeScale);
//
//
//        particleMenu->AddSlider("lifeSpan", &m_particleSys->lifeSpan, 0.0f, 10.0f);
//        particleMenu->AddSlider("lifeSpanVar", &m_particleSys->lifeSpanVar, 0.0f, 10.0f);
//        
//        particleMenu->AddSlider("rotEnd", &m_particleSys->rotEnd,           0.0f, 360.0f, "Rotation");
//        particleMenu->AddSlider("rotEndVar", &m_particleSys->rotEndVar,     0.0f, 360.0f, "Rotation");
//        particleMenu->AddSlider("rotStart", &m_particleSys->rotStart,       0.0f, 360.0f, "Rotation");
//        particleMenu->AddSlider("rotStartVar", &m_particleSys->rotStartVar, 0.0f, 360.0f, "Rotation");
//        
//        particleMenu->AddSlider("sourcePosX", &m_particleSys->sourcePos.x, -posScale, posScale, "SourcePos");
//        particleMenu->AddSlider("sourcePosY", &m_particleSys->sourcePos.y, -posScale, posScale, "SourcePos");
//        particleMenu->AddSlider("sourcePosZ", &m_particleSys->sourcePos.z, -posScale, posScale, "SourcePos");
//        particleMenu->AddSlider("sourcePosVarX", &m_particleSys->sourcePosVar.x, -posScale, posScale, "SourcePosVar");
//        particleMenu->AddSlider("sourcePosVarY", &m_particleSys->sourcePosVar.y, -posScale, posScale, "SourcePosVar");
//        particleMenu->AddSlider("sourcePosVarZ", &m_particleSys->sourcePosVar.z, -posScale, posScale, "SourcePosVar");
//
//        particleMenu->AddSlider("startSize", &m_particleSys->startSize, 0.0f, sizeScale, "StartSize");
//        particleMenu->AddSlider("startSizeVar", &m_particleSys->startSizeVar, 0.0f, sizeScale, "StartSize");
//        
//        if ( m_particleSys->emitterType == ParticleSysGravity ) {
//            particleMenu->AddSlider("gravityX", &m_particleSys->gravity.x, -100.0f, 100.0f, "Gravity");
//            particleMenu->AddSlider("gravityY", &m_particleSys->gravity.y, -100.0f, 100.0f, "Gravity");
//            particleMenu->AddSlider("gravityZ", &m_particleSys->gravity.z, -100.0f, 100.0f, "Gravity");
//            particleMenu->AddSlider("speed", &m_particleSys->speed, -1000.0f, 1000.0f, "Speed");
//            particleMenu->AddSlider("speedVar", &m_particleSys->speedVar, 0.0f, 100.0f, "Speed");
//            particleMenu->AddSlider("radialAccel", &m_particleSys->radialAccel, 0.0f, 100.0f);
//            particleMenu->AddSlider("radialAccelVar", &m_particleSys->radialAccelVar, 0.0f, 100.0f);
//            particleMenu->AddSlider("tangAccel", &m_particleSys->tangAccel, -100.0f, 100.0f);
//            particleMenu->AddSlider("tangAccelVar", &m_particleSys->tangAccelVar, -100.0f, 100.0f);
//        } else /*if ( emitterType == ParticleSysRadial )*/ {
//            particleMenu->AddSlider("maxRadius", &m_particleSys->maxRadius, 0.0f, 100.0f);
//            particleMenu->AddSlider("maxRadiusVar", &m_particleSys->maxRadiusVar, 0.0f, 100.0f);
//            particleMenu->AddSlider("minRadius", &m_particleSys->minRadius, 0.0f, 100.0f);
//            particleMenu->AddSlider("minRadiusVar", &m_particleSys->minRadiusVar, 0.0f, 100.0f);
//            particleMenu->AddSlider("rotPerSec", &m_particleSys->rotPerSec, 0.0f, 100.0f);
//            particleMenu->AddSlider("rotPerSecVar", &m_particleSys->rotPerSecVar, 0.0f, 100.0f);
//        }
//    }
}
void ParticleEditor::RemoveEditor()
{
    std::vector<GUIButton*>::iterator it = buttonVect.begin();
    while (it != buttonVect.end() ) {
        delete (*it);
        it++;
    }
    buttonVect.clear();
    
    if ( cameraBtn ) {
        delete cameraBtn;
        cameraBtn = NULL;
    }
    if (optionsBtn ) {
        delete optionsBtn;
        optionsBtn = NULL;
    }
    if ( fileMenu ) {
        delete fileMenu;
        fileMenu = NULL;
    }
    if ( particleMenu ) {
        delete particleMenu;
        particleMenu = NULL;
    }
}

void ParticleEditor::Update ( double delta )
{
    
    UpdateMovement();
    Locator::getText().Update(delta);
    // Update particle systems
    Locator::getParticles().Update( delta*timeScaler );

}
void ParticleEditor::Draw( void )
{
    camera.position = glm::vec3(16,16,16);
    camera.targetPosition = glm::vec3(16,16,16);
    camera.targetRotation = glm::vec3(-45,45,0);
    camera.rotation = glm::vec3(-45,45,0);
//    RendererBase* renderer = m_hyperVisor.GetRenderer();
//    glPolygonMode( GL_FRONT_AND_BACK, m_hyperVisor.GetOptions()->GetOptionDataPtr<bool>("r_renderWireFrame") ? GL_LINE : GL_FILL );
//    // Draw editing object and floor
//    float objectHeight = 5.0f;
//    float objectWidth = 5.0f;
//    // Render editing table
//    float tableSize = objectHeight > objectWidth ? objectHeight : objectWidth;
//    CubeInstance tableCube = {
//        0.0f,-(tableSize*2.0f)+0.005f,0.0f,objectHeight,
//        0.0f,0.0f,0.0f,1.0f,
//        0.1f,0.1f,0.1f,1.0f,
//        1.0f,1.0f,1.0f,1.0f
//    };
//    // Render editing floor
//    float floorSize = 50.0f;
//    CubeInstance floorCube = {
//        0.0f,-(floorSize+tableSize*3.0f),0.0f,floorSize,
//        0.0f,0.0f,0.0f,1.0f,
//        0.2f,0.2f,0.2f,1.0f,
//        1.0f,1.0f,1.0f,1.0f
//    };
//    glDisable(GL_BLEND);
//    glEnable(GL_STENCIL_TEST);
//    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//    glStencilFunc(GL_ALWAYS, Stencil_Solid, 0xFF);
//    renderer->Buffer3DCube(floorCube);
//    renderer->Buffer3DCube(tableCube);
//    renderer->Render3DCubes();
//    glDisable(GL_STENCIL_TEST);
//
//    // Render particles
////    hyperVisor.GetParticleMan()->Draw(renderer);
//    m_hyperVisor.GetParticleMan()->DrawLitParticles(renderer);
//
//    // Apply lighting
//    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
//    if ( m_hyperVisor.GetOptions()->GetOptionDataPtr<bool>("r_useShaders") && m_hyperVisor.GetOptions()->GetOptionDataPtr<bool>("r_deferred") ) {
//        LightSystem3D* lsys = m_hyperVisor.GetLightSys3D();
//        if ( lsys->GetLights().size() == 0 ) {
//            Light3D* newLight = new Light3D();
//            newLight->attenuation = glm::vec3(0.0f,0.9f,0.5f);
//            newLight->lightType = Light3D_Point;
//            lsys->Add(newLight);
//            newLight = new Light3D();
//            newLight->attenuation = glm::vec3(0.0f,0.9f,0.5f);
//            newLight->lightType = Light3D_Spot;
//            newLight->spotCutoff = 60.0f;
//            lsys->Add(newLight);
//            newLight = new Light3D();
//            newLight->attenuation = glm::vec3(0.0f,0.9f,0.5f);
//            newLight->lightType = Light3D_Point;
//            lsys->Add(newLight);
//        } else {
//            const float radius = 60.0f;
//            lsys->GetLights()[0]->diffuse = COLOR_WHITE;
//            lsys->GetLights()[1]->diffuse = COLOR_WHITE;
//            lsys->GetLights()[2]->diffuse = COLOR_WHITE;
//            lsys->GetLights()[0]->specular = COLOR_WHITE;
//            lsys->GetLights()[1]->specular = COLOR_WHITE;
//            lsys->GetLights()[2]->specular = COLOR_WHITE;
//            lsys->GetLights()[0]->position = glm::vec4(-40.0f,0.0f,40.0f, radius);
//            lsys->GetLights()[1]->position = glm::vec4(10.0f,10.0f,10.0f, radius);
//            lsys->GetLights()[2]->position = glm::vec4(40.0f,0.0f,-40.0f, radius);
//        }
//        renderer->RenderLighting( COLOR_FOG_DEFAULT );
//    }
//    m_hyperVisor.GetParticleMan()->DrawUnlitParticles(renderer);
//
}

//========================================================================
// Main menu Button callback functions
//========================================================================
void ParticleEditor::LoadSystemButtonCB( void*data ) {
//    LocalGame * localGame = new LocalGame();
//    m_hyperVisor.GetSceneManager()->AddActiveScene( localGame );
}
void ParticleEditor::CloseEditorButtonCB( void*data ) {
//    std::string prevState = m_hyperVisor.GetSceneManager()->GetPreviousSceneName();
//    if ( !prevState.empty() ) {
//        m_hyperVisor.GetSceneManager()->SetActiveScene(prevState);
//    }
}

void ParticleEditor::ShowOptionsMenu() {
//    if ( optionsMenu == NULL ) {
//        int bW = 140;
//        int bH = 22;
//        int posX = 8+bW+8;
//        int posY = m_hyperVisor.GetOptions()->GetOptionDataPtr<int>("r_resolutionY")/2-30;
//
//        optionsMenu = new UIMenu(posX, posY, bW, bH, "Options");
//        // Get all the options and add them in to our menu
//        std::map<const std::string, Attribute*>& allOptions = m_hyperVisor.GetOptions()->GetOptionMap();
//        std::map<const std::string, Attribute*>::iterator it;
//        for ( it = allOptions.begin(); it != allOptions.end(); it++ ) {
//            std::string category = "";
//            if ( it->first.substr(0, 2) == "a_" ) { category = "Audio"; }
//            else if ( it->first.substr(0, 2) == "d_" ) { category = "Debug"; }
//            else if ( it->first.substr(0, 2) == "e_" ) { category = "Editor"; }
//            else if ( it->first.substr(0, 2) == "h_" ) { category = "HyperVisor"; }
//            else if ( it->first.substr(0, 2) == "i_" ) { category = "Input"; }
//            else if ( it->first.substr(0, 2) == "r_" ) { category = "Renderer"; }
//            if ( it->second->IsType<bool>()) {
//                optionsMenu->AddVar<bool>(it->first, &it->second->as<bool>(), category);
//            } else if ( it->second->IsType<int>()) {
//                optionsMenu->AddSlider<int>(it->first, &it->second->as<int>(), 0, 100, category);
//            } else if ( it->second->IsType<float>()) {
//                optionsMenu->AddSlider<float>(it->first, &it->second->as<float>(), 0.0f, 100.0f, category);
//            } else if ( it->second->IsType<std::string>()) {
//                optionsMenu->AddVar<std::string>(it->first, &it->second->as<std::string>(), category);
//            }
//        }
//        optionsMenu->AddButton("Defaults", ( [=]() {
//            m_hyperVisor.GetOptions()->ResetToDefaults();
//        }  ) );
//        optionsMenu->AddButton("Save", ( [=]() {
//            m_hyperVisor.GetOptions()->SaveOptions();
//        }  ) );
//        optionsMenu->AddButton("Close", ( [=]() {
//            if ( optionsMenu != NULL ) {
//                delete optionsMenu;
//                optionsMenu = NULL;
//            }
//        }  ) );
//        optionsMenu->Sort();
//    } else {
//        RemoveOptionsMenu();
//    }
}
void ParticleEditor::RemoveOptionsMenu() {
    if ( optionsMenu != NULL ) {
        delete optionsMenu;
        optionsMenu = NULL;
    }
}

//========================
//  Input event handling
//========================
void ParticleEditor::UpdateMovement() {
    float deadZone = 0.35f;
    
    if ( fabsf(joyMoveInput.x)+fabsf(joyMoveInput.y) < deadZone ) joyMoveInput = glm::vec2();
    if ( fabsf(joyRotateInput.x)+fabsf(joyRotateInput.y) < deadZone ) joyRotateInput = glm::vec2();
    
    camera.movement.x = joyMoveInput.x;
    camera.movement.z = -joyMoveInput.y;
    
    float joySensitivity = 2.0f;
    float rotationX = -joyRotateInput.x*joySensitivity;
    float rotationY = joyRotateInput.y*joySensitivity;
    camera.CameraRotate(rotationX, rotationY);
}

bool ParticleEditor::OnEvent(const std::string& event, const float &amount)
{
    if (event == "MoveForward") {
        joyMoveInput.y += amount;
    } else if (event == "MoveBackward") {
        joyMoveInput.y += -amount;
    } else if (event == "MoveLeft") {
        joyMoveInput.x += -amount;
    } else if (event == "MoveRight") {
        joyMoveInput.x += amount;
    } else if (event == "MoveUp") {
        joyMoveInput.y += -amount;
    } else if (event == "MoveDown") {
        joyMoveInput.y += amount;
    }
    else if ( amount == 1.0 ) {
        if ( event == "Shoot" )
        {
//            printf("Editor has been clicked\n");
        }

    } else if ( amount == -1.0 ) {
//        if ( event == "ShowConsole" ) {
//            Console::ToggleVisibility();
//        } else if (event == "GrabCursor" ) {
//            m_hyperVisor.GetOptions()->GetOptionDataPtr<bool>("r_grabCursor") = !m_hyperVisor.GetOptions()->GetOptionDataPtr<bool>("r_grabCursor");
//            if ( m_hyperVisor.GetOptions()->GetOptionDataPtr<bool>("r_grabCursor") ) {
//                glfwSetInputMode(m_hyperVisor.GetRenderer()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//            } else {
//                glfwSetInputMode(m_hyperVisor.GetRenderer()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//            }
//        } else if ( event == "Escape" ) {
//            if ( Console::isVisible() ) {
//                Console::ToggleVisibility();
//            }
//            // Show main menu
//            std::string prevState = m_hyperVisor.GetSceneManager()->GetPreviousSceneName();
//            if ( !prevState.empty() ) {
//                m_hyperVisor.GetSceneManager()->SetActiveScene(prevState);
//            }
//        }
    }
    return false;
}

bool ParticleEditor::OnMouse(const glm::ivec2& coord) {
//    double midWindowX = m_hyperVisor.GetOptions()->GetOptionDataPtr<int>("r_resolutionX") / 2.0;     // Middle of the window horizontally
//    double midWindowY = m_hyperVisor.GetOptions()->GetOptionDataPtr<int>("r_resolutionY") / 2.0;    // Middle of the window vertically
//    if ( m_hyperVisor.GetOptions()->GetOptionDataPtr<bool>("r_grabCursor") ) {
//        
//        float mouseSensitivity = 0.1f;
//        float rotationX = (midWindowX-mx)*mouseSensitivity;
//        float rotationY = (midWindowY-my)*mouseSensitivity;
//        
//        if ( m_hyperVisor.GetCamera().thirdPerson) {
//            rotationX *= -1.0f;
//            rotationY *= -1.0f;
//        }
//        m_hyperVisor.GetCamera().CameraRotate(rotationX, rotationY);
//        // Reset the mouse position to the centre of the window each frame
//        RendererBase* renderer = m_hyperVisor.GetRenderer();
//        glfwSetCursorPos(renderer->GetWindow(), midWindowX, midWindowY);
////        cursor.posScrn = glm::vec2();
//    } else {
////        cursor.posScrn = glm::vec2(mx-midWindowX, midWindowY-my);
//    }
    return false;
}

void ParticleEditor::LoadSystem(const std::string fileName) {
    if ( fileSelectMenu ) { delete fileSelectMenu; fileSelectMenu = NULL; }
    if ( m_particleSys ) {
        Locator::getParticles().RemoveSystem(m_particleSys);
    }

    if ( fileName.length() > 0 ) {
        std::string particlePath = FileUtil::GetPath().append("Data/Particles/");
        size_t fileNPos = fileName.find_last_of("/");
        std::string shortFileName = fileName;
        if ( fileNPos ) shortFileName = fileName.substr(fileNPos+1);
		printf("Loading system: %s from %s - %s\n", fileName.c_str(), particlePath.c_str(), shortFileName.c_str());
        m_particleSys = Locator::getParticles().AddSystem(particlePath, shortFileName);
    }
    ShowEditor();
}

void ParticleEditor::SaveSystem(const std::string fileName) {
    if ( fileSelectMenu ) { delete fileSelectMenu; fileSelectMenu = NULL; }
    if ( !m_particleSys ) { return; }
    if ( fileName.length() > 0 ) {
        std::string particlePath = FileUtil::GetPath().append("Data/Particles/");
        size_t fileNPos = fileName.find_last_of("/");
        std::string shortFileName = fileName;
        if ( fileNPos ) shortFileName = fileName.substr(fileNPos+1);
        m_particleSys->SaveToFile(particlePath, shortFileName);
    }
    ShowEditor();
}

