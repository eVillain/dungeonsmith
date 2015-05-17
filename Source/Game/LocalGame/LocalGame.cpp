//
//  LocalGame.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 10/10/12.
//  Copyright (c) 2014 The Drudgerist. All rights reserved.
//
#include "LocalGame.h"
#include "Locator.h"

LocalGame::LocalGame() : Scene("Game")
{
}

LocalGame::~LocalGame()
{
}

void LocalGame::Initialize()
{
    Scene::Initialize();
    world.Initialize();
}

void LocalGame::ReInitialize()
{
}

void LocalGame::Release()
{
    Scene::Release();
}

void LocalGame::Cleanup()
{
    
}

void LocalGame::Pause( void )
{
    if ( !IsPaused() )
    {
        Scene::Pause();
    }
}

void LocalGame::Resume( void )
{
    if ( IsPaused() )
    {
        Scene::Resume();
    }
}

void LocalGame::Update( double deltaTime )
{
    world.Update(deltaTime);
}

void LocalGame::Draw( void )
{

}
