//
//  MainMenu.cpp
//  DungeonSmith
//
//  Created by eVillain on 29/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "MainMenu.h"
#include "Locator.h"
#include "Input.h"
#include "GUIButton.h"
//#include "Primitives2D.h"
#include "LocalGame.h"

MainMenu::MainMenu() : Scene("Main Menu"),
eventFunctor(this, &MainMenu::OnEvent)
{
    _buttonStartGame = nullptr;
}


MainMenu::~MainMenu()
{

}

void MainMenu::Initialize()
{
    Scene::Initialize();
    
    Input::RegisterEventObserver(&eventFunctor);
    
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
    }
    Input::UnRegisterEventObserver(&eventFunctor);
    
    RemoveButtons();
}

void MainMenu::Resume()
{
    if ( IsPaused() )
    {
        Scene::Resume();
    }
    Input::RegisterEventObserver(&eventFunctor);
    
    CreateButtons();
}

void MainMenu::Update( double deltaTime )
{
}

void MainMenu::Draw()
{
//    Locator::getRenderer().Primitives()->RectFilled(glm::vec2(0,0), glm::vec2(1000,1000), COLOR_WHITE, 2);
}

bool MainMenu::OnEvent(const typeInputEvent& event, const float& amount)
{
    return false;
}

void MainMenu::CreateButtons()
{
    _buttonStartGame = new GUI::GUIButtonLabeled("Start Game", 0,0,200,200,0);
    GUI::ButtonBehaviorLambda* startGameBehavior = new GUI::ButtonBehaviorLambda([](){
        Locator::getSceneManager().AddActiveScene( new LocalGame());
    });
    _buttonStartGame->SetBehavior(startGameBehavior);
}

void MainMenu::RemoveButtons()
{
    if ( _buttonStartGame )
    {
        delete _buttonStartGame;
        _buttonStartGame = nullptr;
    }
}