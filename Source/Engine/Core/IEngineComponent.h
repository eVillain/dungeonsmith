//
//  IEngineComponent.h
//  DungeonSmith
//
//  Created by eVillain on 11/10/15.
//  Copyright © 2015 The Drudgerist. All rights reserved.
//

#ifndef IEngineComponent_h
#define IEngineComponent_h

class IEngineComponent
{
public:
    // Return true for success, false for error
    virtual bool Initialize() = 0;
    virtual bool Terminate() = 0;

    virtual void Update(const double deltaTime) = 0;
};

#endif /* IEngineComponent_h */
