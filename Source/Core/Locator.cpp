//
//  Locator.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 29/01/15.
//  Copyright (c) 2015 BitWiseOperators. All rights reserved.
//

#include "Locator.h"
#include "HyperVisor.h"
#include "SceneManager.h"
#include "Camera.h"
#include "GUIManager.h"
#include "ThreadPool.h"
#include "GUIManager.h"
#include "Physics.h"
#include "Particles.h"

NullRenderer Locator::_nullRenderService;
NullText Locator::_nullTextService;

// Set all services to be null services first
IRenderer* Locator::_renderService = &_nullRenderService;
IText* Locator::_textService = &_nullTextService;
HyperVisor* Locator::_hv = nullptr;

ThreadPool* Locator::_threadPool = nullptr;
GUI::GUIManager Locator::_guiManager;
Physics Locator::_physics;
Particles Locator::_particles;
SceneManager Locator::_sceneManager;
