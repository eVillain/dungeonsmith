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
#include "Manager.h"
#include "ThreadPool.h"
#include "Manager.h"
#include "Physics.h"
#include "Particles.h"
#include "Input.h"

NullRenderer Locator::_nullRenderService;
NullText Locator::_nullTextService;

// Set all services to be null services first
IRenderer* Locator::_renderService = &_nullRenderService;
IText* Locator::_textService = &_nullTextService;
HyperVisor* Locator::_hv = nullptr;
Input Locator::_input;

ThreadPool* Locator::_threadPool = nullptr;
GUI::Manager Locator::_guiManager;
Physics Locator::_physics;
Particles Locator::_particles;
SceneManager Locator::_sceneManager;
