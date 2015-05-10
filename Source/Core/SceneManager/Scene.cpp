//
//  Scene.cpp
//  NeverEngine
//
//  Created by The Drudgerist on 8/28/12.
//  Copyright (c) 2012 The Drudgerist. All rights reserved.
//

#include "Scene.h"

Scene::Scene(const std::string sceneID) :
mSceneID(sceneID),
mInit(false),
mPaused(false) {
}

Scene::~Scene() {
}

const std::string Scene::GetID() const {
    return mSceneID;
}

void Scene::Initialize() {
    // Initialize if necessary
    if(mInit == false) {
        mInit = true;
        mPaused = false;
    }
    
}

void Scene::Release(void) {
    // Clean up if necessary
    if(mInit == true) {
        mCleanup = true;
        mInit = false;
    }
}

void Scene::Pause() {
    if (mPaused == false) {
        mPaused = true;
    }
}

void Scene::Resume() {
    if (mPaused == true) {
        mPaused = false;
    }
}
