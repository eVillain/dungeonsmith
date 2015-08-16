//
//  World.h
//  DungeonSmith
//
//  Created by The Drudgerist on 14/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__World__
#define __DungeonSmith__World__

#include "RenderDefines.h"
#include "Light3D.h"
#include "ChunkManager.h"

class Camera;
class SkyDome;

class World {
public:
    void Initialize();
    void Terminate();
    
    void Update( double deltaTime );
    void Draw();
    Camera& GetCamera() { return _camera; };
private:
    ChunkManager _chunks;
    SkyDome* skyDome;
    Light3D _sunLight;
    Camera _camera;
};

#endif /* defined(__DungeonSmith__World__) */
