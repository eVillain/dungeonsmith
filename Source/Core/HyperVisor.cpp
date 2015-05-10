//
//  HyperVisor.cpp
//  DungeonSmith
//
//  Created by eVillain on 20/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "HyperVisor.h"
#include "Locator.h"
#include "Input.h"
#include "Renderer.h"
#include "CommandProcessor.h"
#include "Console.h"
#include "Timer.h"
#include "TextManager.h"
#include "FileUtil.h"

HyperVisor::HyperVisor() :
quit(false),
errorCode(0),
eventFunctor(this, &HyperVisor::OnEvent)
{
}

HyperVisor::~HyperVisor()
{
}

void HyperVisor::Initialize(int argc, char * arg[])
{
    // Engine init
    if ( !initialized )
    {
        FileUtil::UpdateRelativePath();
        
        // Start up our command processor - needs to be done first so that
        // the engine subsystems can register commands in their initializers.
        CommandProcessor::Initialize();

        Input::Initialize();
        Input::RegisterEventObserver(&eventFunctor);
        Console::Initialize();
        
        // Create and initialize all of our engine subsystems
        Renderer* renderer = new Renderer();
        renderer->Initialize();
        
        TextManager* textMan = new TextManager();
        textMan->Initialize();
        
        // Pass the engine subsystems references to our Service Locator
        Locator::provideHyperVisor(this);
        Locator::provideRenderer(renderer);
        Locator::provideText(textMan);
        
        // Engine is finally ready so we'll pass our command-line arguments to
        // the CommandProcessor where they will be parsed and processed.
        CommandProcessor::Buffer(argc, arg);
        
        initialized = true;
    }
}

void HyperVisor::Terminate()
{
    // Clean up
    if ( initialized )
    {
        CommandProcessor::Terminate();
        
        Console::Terminate();
        
        IRenderer* renderer = &Locator::getRenderer();
        delete renderer;
        
        IText* textMan = &Locator::getText();
        delete textMan;
        
        Locator::provideHyperVisor(NULL);
        Locator::provideRenderer(NULL);
        Locator::provideText(NULL);
        
        initialized = false;
    }
}

int HyperVisor::Run()
{
    double timeAtThisFrameStart = 0.0;
    double deltaTime = 0.0;
    while (!quit)
    {
        timeAtThisFrameStart = Timer::Milliseconds();
        deltaTime = timeAtLastFrameStart - timeAtThisFrameStart;
        timeAtLastFrameStart = timeAtThisFrameStart;
        
        // --- Update stuff --- //
        Input::ProcessInput();
        
        CommandProcessor::Update(deltaTime);
        
        
        Locator::getText().Update(deltaTime);

        // --- Render stuff --- //
        Locator::getRenderer().BeginDraw();

        
        // Test drawing
//        Locator::getRenderer().Primitives()->Line(glm::vec2(), glm::vec2(10,10), COLOR_RED, COLOR_BLUE);
//        Locator::getRenderer().Primitives()->Circle(glm::vec2(-20,-20), 0.0, 20.0, COLOR_YELLOW);
//        Locator::getRenderer().Primitives()->Rectangle(glm::vec2(100,100), glm::vec2(40,30), COLOR_GREEN, 1.0);

        
        Console::Draw(deltaTime);
        
        Locator::getText().RenderLabels();
        
        Locator::getRenderer().EndDraw();
    }
    
    return errorCode;
}

void HyperVisor::Stop(const int reason)
{
    errorCode = reason;
    quit = true;
}

// Low level events - we will try to make sure that we can always access the
// console or at least back to the main menu so we can quit :)
void HyperVisor::OnEvent( typeInputEvent event, float amount )
{
    if ( amount != -1 ) return; // We only care for key/button release events here
    
    if ( event == "console" )
    {
        if ( !Console::isVisible() ) Console::ToggleVisibility();
    }
    else if ( event == "back" )
    {
        if ( Console::isVisible() )
        { Console::ToggleVisibility(); }
        else
        { Stop(); } // For now we stop TODO: change this to pop the previous scene
    }
}

