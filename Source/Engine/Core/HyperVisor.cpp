//
//  HyperVisor.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 20/04/15.
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
#include "Scene.h"
#include "Manager.h"
#include "ThreadPool.h"
#include "SceneManager.h"
#include "Physics.h"

HyperVisor::HyperVisor() :
quit(false),
errorCode(0),
initialized(false)
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
        CommandProcessor::AddCommand("quit", Command<int>([=](int reason){ Stop(reason); } ));
        // TODO: Figure out why the below does not compile
//        CommandProcessor::AddCommand("quit", InstanceCommand<HyperVisor, int>(&HyperVisor::Stop, this));
        
        Console::Initialize();

        const int hwThreads = std::thread::hardware_concurrency();
		const int numThreads = (2*hwThreads)-1;

        ThreadPool* threadPool = new ThreadPool(numThreads);
        
        
        Locator::getInput().Initialize();
        Locator::getInput().RegisterEventObserver(this);
        
        // Create and initialize all of our engine subsystems
        Renderer* renderer = new Renderer();
		if (!renderer->Initialize())
		{
			return; // TODO: Gracefully handle exit in systems with crap OpenGL support :)
		}
        
        TextManager* textMan = new TextManager();
        textMan->Initialize();
        
        // Pass the engine subsystems references to our Service Locator
        Locator::provideHyperVisor(this);
        Locator::provideThreadPool(threadPool);
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
//        Locator::getInput().UnRegisterEventObserver(this);
//        Locator::getInput().Terminate();
        
        CommandProcessor::Terminate();
        
        Console::Terminate();
        
        ThreadPool* threadPool = &Locator::getThreadPool();
        delete threadPool;
        
        IRenderer* renderer = &Locator::getRenderer();
        delete renderer;
        
        IText* textMan = &Locator::getText();
        delete textMan;
        
        Locator::provideHyperVisor(nullptr);
        Locator::provideThreadPool(nullptr);
        Locator::provideRenderer(nullptr);
        Locator::provideText(nullptr);
        
        initialized = false;
    }
}

int HyperVisor::Run()
{
    timeAtLastFrameStart = Timer::Seconds();
    double timeAtThisFrameStart = timeAtLastFrameStart;
    double deltaTime = 0.0;
    while (!quit)
    {
        timeAtThisFrameStart = Timer::Seconds();
        deltaTime = timeAtThisFrameStart - timeAtLastFrameStart;
        timeAtLastFrameStart = timeAtThisFrameStart;
        
        // --- Update stuff --- //
        Locator::getInput().ProcessInput();
        CommandProcessor::Update(deltaTime);
        
        Locator::getGUI().Update(deltaTime);

        Scene& currentScene = Locator::getSceneManager().GetActiveScene();
        currentScene.Update(deltaTime);
        
        Locator::getText().Update(deltaTime);

        // --- Render stuff, deferred 3D first --- //
        Locator::getRenderer().BeginDraw();

        currentScene.Draw();
        
        Locator::getText().Render3D();

        // --- Finish deferred render and switch to forward render --- //
        Locator::getRenderer().FinishDeferred();
        
        if ( Locator::getPhysics().DebugEnabled() )
        {
            Locator::getPhysics().Draw();
        }
        
        Locator::getGUI().Draw();
        
        Console::Draw(deltaTime);

        Locator::getText().Render2D();
        
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
bool HyperVisor::OnEvent( const std::string& event, const float& amount )
{
    if ( amount != -1 ) return false; // We only care for key/button release events here
    
    if ( event == "console" )
    {
            Console::ToggleVisibility();
        return true;
    }
    else if ( event == "back" )
    {
        if ( Console::isVisible() )
        { Console::ToggleVisibility(); }
        else
        { Stop(); } // For now we stop TODO: change this to pop the previous scene
        return true;
    }
    return false;
}

