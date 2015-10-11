//
//  IRenderComponent.h
//  DungeonSmith
//
//  Created by eVillain on 29/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_IRenderComponent_h
#define DungeonSmith_IRenderComponent_h

class IRenderComponent
{
public:
    virtual ~IRenderComponent() {};
    virtual void Initialize() = 0;
    virtual void Terminate() = 0;
};

#endif
