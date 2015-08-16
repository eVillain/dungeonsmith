//
//  ButtonBehavior.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 30/07/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "ButtonBehavior.h"

namespace GUI
{
    ButtonBehaviorLambda::ButtonBehaviorLambda( std::function<void()> func) :
    function(func)
    { }
    
    void ButtonBehaviorLambda::Trigger()
    {
        if ( function )
        {
            function();
        }
    }
    
    ButtonBehaviorToggle::ButtonBehaviorToggle( bool* clientData ) :
    toggleData(clientData)
    { }
    
    void ButtonBehaviorToggle::Trigger()
    {
        if ( toggleData )
        {
            *toggleData = !(*toggleData );
        }
    }
}   /* namespace GUI */
