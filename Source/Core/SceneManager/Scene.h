#ifndef NGN_SCENE_H
#define NGN_SCENE_H
//
//  Scene.h
//  NeverEngine
//
//  Created by Ville-Veikko Urrila on 8/10/12.
//  Copyright (c) 2012 The Drudgerist. All rights reserved.
//

#include <string>

class Scene {
public:
    Scene( const std::string sceneID );
    virtual ~Scene();
    const std::string GetID() const;

    virtual void Initialize();
    virtual void ReInitialize() = 0;
    void Release();
    bool IsInitialized() { return mInit; };
    bool IsPaused() { return mPaused; };
    virtual void Pause();
    virtual void Resume();
    
    virtual void Update( double delta ) = 0;
    virtual void Draw() = 0;
    virtual void Cleanup() = 0;
private:
    // The Scene ID
    const std::string   mSceneID;
    bool                mInit;
    bool                mPaused;
    bool                mCleanup;

    Scene( const Scene& );            // Intentionally undefined constructor
    Scene& operator=( const Scene& ); // Intentionally undefined constructor
};
#endif
