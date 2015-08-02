//
//  Locator.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 29/01/15.
//  Copyright (c) 2015 BitWiseOperators. All rights reserved.
//

#include "Locator.h"

#include "GUIManager.h"


NullRenderer Locator::_nullRenderService;
NullText Locator::_nullTextService;

// Set all services to be null services first
IRenderer* Locator::_renderService = &_nullRenderService;
IText* Locator::_textService = &_nullTextService;
SceneManager Locator::_sceneManager;
HyperVisor* Locator::_hv = nullptr;

ThreadPool* Locator::_threadPool = nullptr;
GUI::GUIManager Locator::_guiManager;