//
//  Locator.h
//  DungeonSmith
//
//  Created by The Drudgerist on 26/01/15.
//  Copyright (c) 2015 BitWiseOperators. All rights reserved.
//
//  Service locator class as a brute method for accessing engine singletons
//

#ifndef NGN_LOCATOR_H
#define NGN_LOCATOR_H

#include "IRenderer.h"
#include "IText.h"

class HyperVisor;
class SceneManager;
class Camera;
class ThreadPool;
class Physics;
class Particles;

namespace GUI {
    class Manager;
}

class Locator {
public:
    static HyperVisor& getHyperVisor(){ return *_hv; };
    // Getters to current services
    static IRenderer& getRenderer() { return *_renderService; }
    static IText& getText() { return *_textService; };
    static SceneManager& getSceneManager() { return _sceneManager; };
    
    static GUI::Manager& getGUI() { return _guiManager; };
    static ThreadPool& getThreadPool() { return *_threadPool; };
    static Physics& getPhysics() { return _physics; };
    static Particles& getParticles() { return _particles; };
    // Setters for configuring services
    static void provideRenderer(IRenderer* service)
    {
        if (service == nullptr) {
            _renderService = (IRenderer*)&_nullRenderService;
        } else {
            _renderService = service;
        }
    }
    static void provideText(IText* service)
    {
        if (service == nullptr) {
            _textService = (IText*)&_nullTextService;
        } else {
            _textService = service;
        }
    }
    static void provideHyperVisor(HyperVisor* service)
    {
        _hv = service;
    }
    static void provideThreadPool(ThreadPool* service)
    {
        _threadPool = service;
    }
    
private:
    // Renderer
    static IRenderer* _renderService;
    static NullRenderer _nullRenderService;
    // Text manager
    static IText* _textService;
    static NullText _nullTextService;
    
    // Hypervisor, should be removed for release build
    static HyperVisor* _hv;
    // Thread pool
    static ThreadPool* _threadPool;
    // Graphical User Interface, should always be available
    static GUI::Manager _guiManager;
    // Physics engine, should always be available
    static Physics _physics;
    // Particle engine, should always be available
    static Particles _particles;
    // Scene manager, should always be available
    static SceneManager _sceneManager;
};

#endif /* defined(NGN_LOCATOR_H) */

