//
//  MainMenu.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 29/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "MainMenu.h"
#include "Locator.h"
#include "SceneManager.h"
#include "Input.h"
#include "Button.h"
#include "Planetoid.h"
#include "LocalGame.h"
#include "Editor.h"
#include "CommandProcessor.h"

MainMenu::MainMenu() : Scene("Main Menu")
{
    _buttonStartGame = nullptr;
    _buttonPlanetoid = nullptr;
    _buttonStartEditor = nullptr;
}


MainMenu::~MainMenu()
{

}

void MainMenu::Initialize()
{
    Scene::Initialize();
    
    Locator::getInput().RegisterEventObserver(this);
    
    CreateButtons();
}

void MainMenu::ReInitialize()
{
}

void MainMenu::Release()
{
    Scene::Release();
}

void MainMenu::Cleanup()
{
}

void MainMenu::Pause()
{
    if ( !IsPaused() )
    {
        Scene::Pause();
        
        Locator::getInput().UnRegisterEventObserver(this);
        
        RemoveButtons();
    }
}

void MainMenu::Resume()
{
    if ( IsPaused() )
    {
        Scene::Resume();
        
        Locator::getInput().RegisterEventObserver(this);
        
        CreateButtons();
    }
}

void MainMenu::Update( double deltaTime )
{
}

void MainMenu::Draw()
{
//    Locator::getRenderer().Primitives()->RectFilled(glm::vec2(0,0), glm::vec2(1000,1000), COLOR_WHITE, 2);
}

bool MainMenu::OnEvent(const std::string& event, const float& amount)
{
    return false;
}

void MainMenu::CreateButtons()
{
    _buttonStartGame = new GUI::ButtonLabeled("Start Game",
                                                 glm::ivec2(0,0),
                                                 glm::ivec2(200,40),
                                                 0);
    GUI::ButtonBehaviorLambda* startGameBehavior = new GUI::ButtonBehaviorLambda([](){
        Locator::getSceneManager().AddActiveScene( new LocalGame());
    });
    _buttonStartGame->SetBehavior(startGameBehavior);
    
    _buttonPlanetoid = new GUI::ButtonLabeled("Planetoid",
                                                 glm::ivec2(0,40),
                                                 glm::ivec2(200,40),
                                                 0);
    GUI::ButtonBehaviorLambda* startPlanetoidBehavior = new GUI::ButtonBehaviorLambda([](){
        Locator::getSceneManager().AddActiveScene( new Planetoid());
    });
    _buttonPlanetoid->SetBehavior(startPlanetoidBehavior);

    _buttonStartEditor = new GUI::ButtonLabeled("Editor",
                                                   glm::ivec2(0,80),
                                                   glm::ivec2(200,40),
                                                   0);
    GUI::ButtonBehaviorLambda* startEditorBehavior = new GUI::ButtonBehaviorLambda([](){
        Locator::getSceneManager().AddActiveScene( new Editor());
    });
    _buttonStartEditor->SetBehavior(startEditorBehavior);

    _buttonExit = new GUI::ButtonLabeled("Exit",
                                                glm::ivec2(0,-40),
                                                glm::ivec2(200,40),
                                                0);
    GUI::ButtonBehaviorLambda* exitBehavior = new GUI::ButtonBehaviorLambda([](){
        CommandProcessor::Buffer("quit");
    });
    _buttonExit->SetBehavior(exitBehavior);
}

void MainMenu::RemoveButtons()
{
    if ( _buttonStartGame )
    {
        delete _buttonStartGame;
        _buttonStartGame = nullptr;
    }
    if ( _buttonPlanetoid )
    {
        delete _buttonPlanetoid;
        _buttonPlanetoid = nullptr;
    }
    if ( _buttonStartEditor )
    {
        delete _buttonStartEditor;
        _buttonStartEditor = nullptr;
    }
    if ( _buttonExit )
    {
        delete _buttonExit;
        _buttonExit = nullptr;
    }
}
